#include "ChooseGameModeScene.hpp"
#include "../Game.hpp"

namespace TDC
{
	void ChooseGameModeScene::init()
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

		_randomMap = std::make_unique<TextButton>(
			sf::Vector2f(50, 33)
			, sf::Vector2f(50, 33)
			, "Random map"
			, sf::Color::Blue
			, sf::Color::Green
			, 20);
		_randomMap->setParent(this);

		_randomMap->setOnClickCallback([&](const sf::Vector2i &)
		{
			_game->setPlayMode("__RANDOM__");
			return false;
		});


		_loadMap = std::make_unique<TextButton>(
			sf::Vector2f(50, 66)
			, sf::Vector2f(50, 33)
			, "Load map"
			, sf::Color::Red
			, sf::Color::Yellow
			, 20);
		_loadMap->setParent(this);

		_loadMap->setOnClickCallback([&](const sf::Vector2i &)
		{
			_game->setLoadMenu();
			return false;
			//			_game->setPlayMode("../TestMap.bin");
		});

	}

	void ChooseGameModeScene::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_loadMap->update(dt, renderWindow);
		_randomMap->update(dt, renderWindow);
	}

	ChooseGameModeScene::~ChooseGameModeScene()
	{}
}