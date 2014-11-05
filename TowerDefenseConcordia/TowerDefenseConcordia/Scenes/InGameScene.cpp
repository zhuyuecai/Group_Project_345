#include "InGameScene.hpp"
#include "../Game.hpp"

namespace TDC
{
	std::size_t InGameScene::IState::__idCounter = 0;

	InGameScene::InGameScene()
	{}

	InGameScene::~InGameScene()
	{}

	void InGameScene::init()
	{
		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea(m->size.x, m->size.y);
		});

		_map.setParent(this);

		_arial.loadFromFile("../assets/arial.ttf");

		_createTowerBtnType1 = std::make_unique<TextButton>(
			sf::Vector2f(80, 0)
			, sf::Vector2f(20, 10)
			, "Create Minigun\n" + std::to_string(TowerType::Type<TowerType::MiniGun>::getCreationPrice()) + "$"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_createTowerBtnType1->setCentered(false);
		_createTowerBtnType1->setParent(this);
		_createTowerBtnType1->setOnClickCallback([&](const sf::Vector2i &)
		{
			_state = std::make_unique<PlaceMinigun>();
			return true;
		});

		_createTowerBtnType2 = std::make_unique<TextButton>(
			sf::Vector2f(80, 10)
			, sf::Vector2f(20, 10)
			, "Create Rocket\n" + std::to_string(TowerType::Type<TowerType::Rocket>::getCreationPrice()) + "$"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_createTowerBtnType2->setCentered(false);
		_createTowerBtnType2->setParent(this);
		_createTowerBtnType2->setOnClickCallback([&](const sf::Vector2i &)
		{
			_state = std::make_unique<PlaceRocket>();
			return true;
		});

		this->setOnClickCallback([&](const sf::Vector2i &p)
		{
			auto index = _map.getCellIndexFromPixels(p.x, p.y);
			if (index == INVALID)
				return true;
			if (_state)
			{
				if (_map.getCell(index)->getType() != CellType::Wall)
					return true;
				auto stateId = _state->id;
				if (stateId == PlaceRocket::getId())
				{
					if (_towers.find(index) != std::end(_towers))
						return true;
					_towers[index] = std::make_unique<Tower<TowerType::Rocket>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
				else if (stateId == PlaceMinigun::getId())
				{
					if (_towers.find(index) != std::end(_towers))
						return true;
					_towers[index] = std::make_unique<Tower<TowerType::MiniGun>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
				else if (stateId == ShowTowerInfos::getId())
				{
					_state.reset();
					_towerInfos.reset(); _sellBtn.reset(); _upgradeBtn.reset();
					if (_towers.find(index) == std::end(_towers))
						return true;
					_state = std::make_unique<ShowTowerInfos>(index);
				}
			}
			else // if !_state
			{
				if (_towers.find(index) == std::end(_towers))
					return true;
				_state = std::make_unique<ShowTowerInfos>(index);
			}
			return true;
		});

	}

	void InGameScene::generate()
	{
		auto w = 10;
		auto h = 10;
		_map.config(w, h, rand() % (h - 1), rand() % (h - 1));
		_map.generate(TDC::PathGenerationOption::Straight);
		_critters.clear();
		this->setRootArea(this->_parentBox.width, this->_parentBox.height);
	}

	void InGameScene::load(const std::string &path)
	{
		if (!_map.loadFromBinary(path))
			generate();
		_map.setPosition({ 0, 0 });
		_map.setDimension({ 80, 100 });
		_critters.clear();
	}

	void InGameScene::update(const sf::Time &dt, sf::RenderWindow *window)
	{

		if (rand() % 15 == 0)
		{
			if (rand() % 3 == 0)
			{
				_critters.emplace_back<Critter>(Critter::CritterType::Type1);
				_critters.back().addSubscriber(_map.getHandle());
				_critters.back().init();
			}
			else
			{
				_critters.emplace_back<Critter>(Critter::CritterType::Type2);
				_critters.back().addSubscriber(_map.getHandle());
				_critters.back().init();
			}
		}

		_map.update(dt, window);
		auto _cellSizeRatio = _map.getCellRatio();

		sf::CircleShape circle;
		circle.setRadius(_cellSizeRatio / 2.0f);
		circle.setPointCount(8);

		for (std::size_t i = 0; i < _critters.size(); ++i)
		{
			if (!_critters[i].update(dt))
			{
				if (i < _critters.size() - 1)
				{
					std::swap(_critters.at(i), _critters.at(_critters.size() - 1));
					_critters.pop_back();
					--i;
				}
				else
					_critters.pop_back();
			}
			else
			{
				circle.setFillColor(_critters[i].getColor());

				auto &c = _critters[i];
				auto from = c.getCurrentCell();
				auto to = c.getDestinationCell();
				auto ratio = c.getCellStage();
				int dif = (int)to - (int)from;
				float r = _cellSizeRatio * ratio;
				if (dif < 0)
					r *= -1.0f;

				if (std::abs(dif) > 1) // vertical move
				{
					auto x = (from % _map.getWidth()) * 1.0f * _cellSizeRatio;
					auto y = (from / _map.getWidth()) * 1.0f * _cellSizeRatio + r;
					circle.setPosition({ x, y });
				}
				else // horizontal move
				{
					auto x = (from % _map.getWidth()) * 1.0f * _cellSizeRatio + r;
					auto y = (from / _map.getWidth()) * 1.0f * _cellSizeRatio;
					circle.setPosition({ x, y });
				}

				window->draw(circle);
			}
		}

		sf::Text text("Key G to generate a new map", _arial, 20);
		text.setColor(sf::Color::Red);
		window->draw(text);

		_createTowerBtnType1->update(dt, window);
		_createTowerBtnType2->update(dt, window);

		sf::CircleShape triangle(_map.getCellRatio() / 2.0f, 3);
		for (auto &e : _towers)
		{
			triangle.setFillColor(e.second->getColor());
			auto pos = _map.getPixelPositionForCell(e.second->getCellIndex());
			triangle.setPosition(pos.x, pos.y);
			window->draw(triangle);
		}

		if (_state && _state->id == ShowTowerInfos::getId())
		{
			auto &e = _towers[static_cast<ShowTowerInfos*>(_state.get())->index];
			_towerInfos = std::make_unique<TextButton>(
				sf::Vector2f(80, 20), sf::Vector2f(20, 80)
				, "Level : "
				+ std::to_string(e->getLevel())
				+ "\nPower : "
				+ std::to_string(e->getPower())
				+ "\nRange : "
				+ std::to_string(e->getRange())
				+ "\nFire rate : "
 				+ std::to_string(e->getFireRate())
				, e->getColor());
			_towerInfos->setCentered(false);
			_towerInfos->setParent(this);


			_sellBtn = std::make_unique<TextButton>(
				sf::Vector2f(0, 50)
				, sf::Vector2f(100, 25)
				, "Sell for " + std::to_string(e->getPrice()) + "$"
				, e->getColor()
				, sf::Color::Green);
			_sellBtn->setCentered(false);
			_sellBtn->setParent(_towerInfos.get());
			_sellBtn->update(dt, window);
			_sellBtn->setOnClickCallback([&](const sf::Vector2i &)
			{
				_towers.erase(e->getCellIndex());
				_state.reset();
				return false;
			});

			if (e->isUpgradable())
			{
				_upgradeBtn = std::make_unique<TextButton>(sf::Vector2f(0, 75),sf::Vector2f(100, 25)
					, "Upgrade to level " + std::to_string(e->getLevel() + 1)
					+ "\n" + std::to_string(e->getNextPrice()) + "$"
					, e->getColor()
					, sf::Color::White );
				_upgradeBtn->setCentered(false);
				_upgradeBtn->setParent(_towerInfos.get());
				_upgradeBtn->update(dt, window);
				_upgradeBtn->setOnClickCallback([&](const sf::Vector2i &)
				{
					e->upgradeLevel();
					return true;
				});
				//removeSubscriber(_upgradeBtn->getHandle());
			}
		if (_towerInfos)
			_towerInfos->update(dt, window);
		if (_sellBtn)
			_sellBtn->update(dt, window);
		if (_upgradeBtn)
			_upgradeBtn->update(dt, window);
		}


	}

	bool InGameScene::_event(const sf::Event &event)
	{
		if (event.type == sf::Event::KeyReleased)
		{
		    if (event.key.code == sf::Keyboard::G)
			{
				generate();
				_towers.clear();
				_state.reset();
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				_game->setMainMenu();
				return false;
			}
		}
		return true;
	}
}