#include "InGameScene.hpp"
#include "../Game.hpp"
#include "../Singleton.hh"

namespace TDC
{
	std::size_t InGameScene::IState::__idCounter = 0;

	InGameScene::InGameScene()
		: _money(300)
	{}

	InGameScene::~InGameScene()
	{}

	void InGameScene::init()
	{
		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea((float)m->size.x, (float)m->size.y);
		});

		_map.setParent(this);

		_moneyBtn = std::make_unique<TextButton>(
			sf::Vector2f(80, 0)
			, sf::Vector2f(20, 10)
			, std::to_string(_money) + " $"
			, sf::Color::Green
			, sf::Color::Magenta
			, 20);
		_moneyBtn->setCentered(false);
		_moneyBtn->setParent(this);

		_createTowerBtnType1 = std::make_unique<TextButton>(
			sf::Vector2f(80, 10)
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
			sf::Vector2f(80, 20)
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

		_createTowerBtnType3 = std::make_unique<TextButton>(
			sf::Vector2f(80, 30)
			, sf::Vector2f(20, 10)
			, "Create Laser\n" + std::to_string(TowerType::Type<TowerType::Laser>::getCreationPrice()) + "$"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_createTowerBtnType3->setCentered(false);
		_createTowerBtnType3->setParent(this);
		_createTowerBtnType3->setOnClickCallback([&](const sf::Vector2i &)
		{
			_state = std::make_unique<PlaceLaser>();
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
					if (_money < TowerType::Rocket::getCreationPrice())
					{
						_state.reset();
						return true;
					}
					_money -= TowerType::Rocket::getCreationPrice();
					_towers[index] = std::make_unique<Tower<TowerType::Rocket>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
				else if (stateId == PlaceMinigun::getId())
				{
					if (_towers.find(index) != std::end(_towers))
						return true;
					if (_money < TowerType::MiniGun::getCreationPrice())
					{
						_state.reset();
						return true;
					}
					_money -= TowerType::MiniGun::getCreationPrice();
					_towers[index] = std::make_unique<Tower<TowerType::MiniGun>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
				else if (stateId == PlaceLaser::getId())
				{
					if (_towers.find(index) != std::end(_towers))
						return true;
					if (_money < TowerType::Laser::getCreationPrice())
					{
						_state.reset();
						return true;
					}
					_money -= TowerType::Laser::getCreationPrice();
					_towers[index] = std::make_unique<Tower<TowerType::Laser>>();
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
		sf::Text critterText("", *Singleton<sf::Font>::getInstance(), 20);
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
			if (!_critters[i].update(dt) || _critters[i].getLife() <= 0.0f)
			{
				if (_critters[i].getLife() <= 0.0f)
				{
					_money += 3;
				}
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

				float x, y;
				if (std::abs(dif) > 1) // vertical move
				{
					x = (from % _map.getWidth()) * 1.0f * _cellSizeRatio;
					y = (from / _map.getWidth()) * 1.0f * _cellSizeRatio + r;
				}
				else // horizontal move
				{
					x = (from % _map.getWidth()) * 1.0f * _cellSizeRatio + r;
					y = (from / _map.getWidth()) * 1.0f * _cellSizeRatio;
				}
				circle.setPosition({ x, y });

				x += _cellSizeRatio / 2.0f;
				y += _cellSizeRatio / 2.0f;
				_critters[i].setPosition({ x, y });

				window->draw(circle);
				critterText.setString(std::to_string((int)_critters[i].getLife()));
				critterText.setPosition({x - 10, y - 10});
				window->draw(critterText);
			}
		}

		sf::Text text("Key G to generate a new map", *Singleton<sf::Font>::getInstance(), 20);
		text.setColor(sf::Color::Red);
		window->draw(text);

		_createTowerBtnType1->update(dt, window);
		_createTowerBtnType2->update(dt, window);
		_createTowerBtnType3->update(dt, window);
		_moneyBtn->setText(std::to_string(_money) + " $");
		_moneyBtn->update(dt, window);

		sf::CircleShape triangle(_map.getCellRatio() / 2.0f, 3);
		for (auto &e : _towers)
		{
			e.second->updateFireCounter(dt.asSeconds());
			triangle.setFillColor(e.second->getColor());
			auto pos = _map.getPixelPositionForCell(e.second->getCellIndex());
			triangle.setPosition((float)pos.x, (float)pos.y);
			sf::Vector2f center(pos.x + _cellSizeRatio / 2.0f, pos.y + _cellSizeRatio / 2.0f);
			float dist = e.second->getRange() * _cellSizeRatio;
			e.second->_critters.clear();
			for (auto &c : _critters)
			{
				auto v = sqrt((c.getPosition().x - center.x)*(c.getPosition().x - center.x) + (c.getPosition().y - center.y)*(c.getPosition().y - center.y));
				if (v <= dist)
				{
					e.second->_critters.push_back(&c);
				}
			}

			window->draw(triangle);

			auto target = e.second->shoot(center);
			if (target != nullptr)
			{
				sf::Vertex line[] =
				{
					sf::Vertex(center),
					sf::Vertex(target->getPosition())
				};
				window->draw(line, 2, sf::Lines);
				target->setDammage(e.second->getPower());
			}
		}

		if (_state && _state->id == ShowTowerInfos::getId())
		{
			auto &e = _towers[static_cast<ShowTowerInfos*>(_state.get())->index];
			_towerIndex = e->getCellIndex();
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
				, "Sell for " + std::to_string(e->getRefund()) + "$"
				, e->getColor()
				, sf::Color::Green);
			_sellBtn->setCentered(false);
			_sellBtn->setParent(_towerInfos.get());
			_sellBtn->update(dt, window);
			_sellBtn->setOnClickCallback([&](const sf::Vector2i &)
			{
				if (_towers.find(_towerIndex) == std::end(_towers))
					return false;
				_money += _towers[_towerIndex]->getRefund();
				_towers.erase(_towerIndex);
				_state.reset();
				return false;
			});

			if (e->isUpgradable())
			{
				_upgradeBtn = std::make_unique<TextButton>(sf::Vector2f(0, 75), sf::Vector2f(100, 25)
					, "Upgrade to level " + std::to_string(e->getLevel() + 1)
					+ "\n" + std::to_string(e->getNextPrice()) + "$"
					, e->getColor()
					, sf::Color::White);
				_upgradeBtn->setCentered(false);
				_upgradeBtn->setParent(_towerInfos.get());
				_upgradeBtn->update(dt, window);
				_upgradeBtn->setOnClickCallback([&](const sf::Vector2i &)
				{
					if (e->getNextPrice() > _money)
						return false;
					_money -= e->getNextPrice();

					e->upgradeLevel();
					return true;
				});
				//removeSubscriber(_upgradeBtn->getHandle());
			}
			else
				_upgradeBtn = nullptr;
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