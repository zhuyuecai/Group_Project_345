#include "MapCreationMenu.hpp"

namespace TDC
{
	void MapCreationMenuBehaviour::init()
	{
		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			publish<Msg::Resize>(*static_cast<const Msg::Resize*>(msg));
		});

		subcribeToMessage<Msg::Event>([this](const IMessage *msg)
		{
			publish<Msg::Event>(*static_cast<const Msg::Event*>(msg));
		});

		_mapName = std::make_unique<TextInput>(
			sf::Vector2u(50, 66)
			, sf::Vector2u(50, 33)
			, "MapName"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_mapName->init();
		addSubscriber(_mapName->getHandle());
	}

	void MapCreationMenuBehaviour::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_mapName->update(dt, renderWindow);
	}

	MapCreationMenuBehaviour::~MapCreationMenuBehaviour()
	{}
}