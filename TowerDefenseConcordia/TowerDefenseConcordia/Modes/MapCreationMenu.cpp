#include "MapCreationMenu.hpp"

namespace TDC
{
	void MapCreationMenuBehaviour::init()
	{

		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea(m->size.x, m->size.y);
		});

		subcribeToMessage<Msg::Event>([this](const IMessage *msg)
		{
			publish<Msg::Event>(*static_cast<const Msg::Event*>(msg));
		});

		_mapName = std::make_unique<TextInput>(
			sf::Vector2u(50, 10)
			, sf::Vector2u(50, 10)
			, "MapName"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_mapName->setParent(this);

		_mapWidth = std::make_unique<NumberInput>(
			sf::Vector2u(50, 25)
			, sf::Vector2u(50, 10)
			, sf::Color::Blue
			, sf::Color::Magenta
			, 20);
		_mapWidth->setParent(this);


		_mapHeight = std::make_unique<NumberInput>(
			sf::Vector2u(50, 40)
			, sf::Vector2u(50, 10)
			, sf::Color::Blue
			, sf::Color::Magenta
			, 20);
		_mapHeight->setParent(this);
	}

	void MapCreationMenuBehaviour::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_mapName->update(dt, renderWindow);
		_mapWidth->update(dt, renderWindow);
		_mapHeight->update(dt, renderWindow);
	}

	MapCreationMenuBehaviour::~MapCreationMenuBehaviour()
	{}
}