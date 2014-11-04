#include "LaunchModeBehaviour.hpp"

namespace TDC
{
		void LaunchModeBehaviour::init()
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

			_play = std::make_unique<TextButton>(
				sf::Vector2f(50, 33)
				, sf::Vector2f(50, 33)
				, "Play !"
				, sf::Color::Blue
				, sf::Color::Green
				, 20);
			_play->setParent(this);

			_play->setOnClickCallback([&](){
				this->publish<Msg::PlayMode>(Msg::PlayMode::Mode::ChoosePlayModeMenu, "");
			});


			_edit = std::make_unique<TextButton>(
				sf::Vector2f(50, 66)
				, sf::Vector2f(50, 33)
				, "Edit ! (Coming soon)"
				, sf::Color::Red
				, sf::Color::Yellow
				, 20);
			_edit->setParent(this);

			_edit->setOnClickCallback([&](){
				this->publish<Msg::PlayMode>(Msg::PlayMode::Mode::CreateMapMenu, "");
			});

		}

		void LaunchModeBehaviour::update(const sf::Time &dt, sf::RenderWindow *renderWindow)
		{
			_play->update(dt, renderWindow);
			_edit->update(dt, renderWindow);
		}

		LaunchModeBehaviour::~LaunchModeBehaviour()
		{}
}