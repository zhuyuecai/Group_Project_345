#include "ChooseGameModeBehaviour.hpp"

namespace TDC
{
	void ChooseGameModeBehaviour::init()
	{
		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			publish<Msg::Resize>(*static_cast<const Msg::Resize*>(msg));
		});

		subcribeToMessage<Msg::Event>([this](const IMessage *msg)
		{
			publish<Msg::Event>(*static_cast<const Msg::Event*>(msg));
		});

		_randomMap = std::make_unique<TextButton>(
			sf::Vector2u(50, 33)
			, sf::Vector2u(50, 33)
			, "Random map"
			, sf::Color::Blue
			, sf::Color::Green
			, 20);
		_randomMap->init();
		addSubscriber(_randomMap->getHandle());

		_randomMap->setOnClickCallback([&](){
			this->publish<Msg::PlayMode>(Msg::PlayMode::Mode::Play, "__RANDOM__");
		});


		_loadMap = std::make_unique<TextButton>(
			sf::Vector2u(50, 66)
			, sf::Vector2u(50, 33)
			, "Load map !\n(testMapBinary.bin only for the moment)"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_loadMap->init();
		addSubscriber(_loadMap->getHandle());

		_loadMap->setOnClickCallback([&](){
			this->publish<Msg::PlayMode>(Msg::PlayMode::Mode::Play, "testMapBinary.bin");
		});

	}

	void ChooseGameModeBehaviour::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_loadMap->update(dt, renderWindow);
		_randomMap->update(dt, renderWindow);
	}

	ChooseGameModeBehaviour::~ChooseGameModeBehaviour()
	{}
}