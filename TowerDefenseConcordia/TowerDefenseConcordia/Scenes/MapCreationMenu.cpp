#include "MapCreationMenu.hpp"
#include "../Game.hpp"

namespace TDC
{
	void MapCreationMenuBehaviour::init()
	{

		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea((float)m->size.x, (float)m->size.y);
		});

		_mapName = std::make_unique<TextInput>(
			sf::Vector2f(50, 10)
			, sf::Vector2f(50, 10)
			, "TestMap"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_mapName->setParent(this);

		_mapWidth = std::make_unique<NumberInput>(
			sf::Vector2f(50, 25)
			, sf::Vector2f(50, 10)
			, sf::Color::Blue
			, sf::Color::Magenta
			, 20);
		_mapWidth->setParent(this);


		_mapHeight = std::make_unique<NumberInput>(
			sf::Vector2f(50, 40)
			, sf::Vector2f(50, 10)
			, sf::Color::Blue
			, sf::Color::Magenta
			, 20);
		_mapHeight->setParent(this);

		_okay = std::make_unique<TextButton>(
			sf::Vector2f(50, 55)
			, sf::Vector2f(50, 10)
			, "Okay"
			, sf::Color::Green
			, sf::Color::Black
			, 20);
		_okay->setParent(this);

		_okay->setOnClickCallback([&](const sf::Vector2i &){
			if (_mapName->getValue().size() == 0)
				return true;
			_game->setMapCreationMode(_mapName->getValue(), _mapWidth->getInt(), _mapHeight->getInt());
			return true;
		});
	}

	void MapCreationMenuBehaviour::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_mapName->update(dt, renderWindow);
		_mapWidth->update(dt, renderWindow);
		_mapHeight->update(dt, renderWindow);
		_okay->update(dt, renderWindow);
	}

	MapCreationMenuBehaviour::~MapCreationMenuBehaviour()
	{}

	bool MapCreationMenuBehaviour::_event(const sf::Event &event)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			_game->setMainMenu();
			return false; // this will be destroyed
		}
		return true;
	}
}