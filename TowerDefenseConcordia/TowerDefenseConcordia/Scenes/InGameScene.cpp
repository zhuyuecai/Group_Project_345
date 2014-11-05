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
			, "Create Minigun"
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
			, "Create Rocket"
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
			if (_towers.find(index) != std::end(_towers))
				return true;
			if (_state)
			{
				auto stateId = _state->id;
				if (stateId == PlaceRocket::getId())
				{
					_towers[index] = std::make_unique<Tower<TowerType::Rocket>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
				else if (stateId == PlaceMinigun::getId())
				{
					_towers[index] = std::make_unique<Tower<TowerType::MiniGun>>();
					_towers[index]->setCellIndex(index);
					_state.reset();
				}
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
		_map.update(dt, window);
		auto _cellSizeRatio = _map.getCellRatio();

		sf::CircleShape circle;
		circle.setRadius(_cellSizeRatio / 2.0f);

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

		sf::Text text("Key 1 to create a critter of type 1.\n\
Key 2 to create a critter of type 2.\n\
Key G to generate a new map\n\
Key ESC to go back to menu.", _arial, 20);
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

	}

	bool InGameScene::_event(const sf::Event &event)
	{
		if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::Num1)
			{
				_critters.emplace_back<Critter>(Critter::CritterType::Type1);
				_critters.back().addSubscriber(_map.getHandle());
				_critters.back().init();
			}
			else if (event.key.code == sf::Keyboard::Num2)
			{
				_critters.emplace_back<Critter>(Critter::CritterType::Type2);
				_critters.back().addSubscriber(_map.getHandle());
				_critters.back().init();
			}
			else if (event.key.code == sf::Keyboard::G)
			{
				generate();
				_towers.clear();
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