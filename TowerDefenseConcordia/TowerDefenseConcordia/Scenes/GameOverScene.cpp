#include "GameOverScene.hpp"
#include "../Game.hpp"

namespace TDC
{
	void GameOver::init()
	{

		subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::Resize *>(msg);
			this->setRootArea((float)m->size.x, (float)m->size.y);
		});

		subcribeToMessage<Msg::Event>([this](const IMessage *msg)
		{
			publish<Msg::Event>(*static_cast<const Msg::Event*>(msg));
		});

		_text = std::make_unique<TextButton>(
			sf::Vector2f(50, 33)
			, sf::Vector2f(50, 33)
			, "Game Over"
			, sf::Color::Blue
			, sf::Color::Green
			, 20);
		_text->setParent(this);

		_text->setOnClickCallback([&](const sf::Vector2i &)
		{
			_game->setMainMenu();
			return false;
		});
	}

	void GameOver::setScore(int s)
	{
		_text->setText("Game Over ! You survive " + std::to_string(s) + " waves !");
	}

	void GameOver::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
	{
		_text->update(dt, renderWindow);
	}

	GameOver::~GameOver()
	{}
}