#include "InGameScene.hpp"

namespace TDC
{
	InGameScene::InGameScene()
	{}

	InGameScene::~InGameScene()
	{}

	void InGameScene::init()
	{
		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			computeCellSizeRatio(static_cast<const Msg::Resize*>(msg)->size);
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea(m->size.x, m->size.y);
		});

		subcribeToMessage<Msg::Event>([this](const IMessage *msg)
		{
			_events(static_cast<const Msg::Event*>(msg)->event);
		});

		_map.setParent(this);

		_arial.loadFromFile("../assets/arial.ttf");
	}

	void InGameScene::generate()
	{
		auto w = rand() % 100 + 10;
		auto h = rand() % 100 + 10;
		_map.config(w, h, rand() % (h - 1), rand() % (h - 1));
		_map.generate(TDC::PathGenerationOption::Straight);
		_critters.clear();
	}

	void InGameScene::load(const std::string &path)
	{
		if (!_map.loadFromBinary(path))
			generate();
		_map.setPosition({ 0, 0 });
		_map.setDimension({ 80, 80 });
		_critters.clear();
	}

	void InGameScene::update(const sf::Time &dt, sf::RenderWindow *window)
	{
		auto &mapArray = _map.getArray();
		auto w = _map.getWidth();
		auto h = _map.getHeight();

		sf::RectangleShape rectangle;
		rectangle.setFillColor(sf::Color(150, 50, 250));
		rectangle.setSize(sf::Vector2f((float)_cellSizeRatio, (float)_cellSizeRatio));

		sf::CircleShape circle;
		circle.setRadius(_cellSizeRatio / 2.0f);

		for (std::size_t i = 0; i < mapArray.size(); ++i)
		{
			auto &e = mapArray[i];
			if (e.getType() == CellType::Wall)
			{
				rectangle.setPosition((float)((i % w) * _cellSizeRatio), (float)((i / w) * _cellSizeRatio));
				window->draw(rectangle);
			}
		}

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
	}

	void InGameScene::_events(const sf::Event &event)
	{
		publish<Msg::Event>(event);

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
			}
			else if (event.key.code == sf::Keyboard::Escape)
			{
				publish<Msg::PlayMode>(Msg::PlayMode::MainMenu, "");
			}
		}
	}

	void InGameScene::computeCellSizeRatio(sf::Vector2u size)
	{
		auto w = _map.getWidth();
		auto h = _map.getHeight();

		_cellSizeRatio = size.x / w;
		_cellSizeRatio = size.y / h < _cellSizeRatio ? size.y / h : _cellSizeRatio;
		_map.setCellSizeRatio(_cellSizeRatio);
	}
}