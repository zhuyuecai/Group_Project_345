#pragma once

#include "Message.hpp"
#include "Cell.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace TDC
{

	typedef sf::Rect<float> Rect;

	namespace Msg
	{
		// Message sent when window is resized
		struct Resize : Message<Resize>
		{
			Resize(const sf::Vector2u &_size)
				: size(_size)
			{}

			sf::Vector2u size;
		};

		// Event message
		struct Event : Message < Event >
		{
			sf::Event event;
			Event(const sf::Event &e)
				: event(e)
			{}
		};

		// Playmode message used to change play modes / menus
		struct PlayMode : Message < PlayMode >
		{
			enum Mode
			{
				MainMenu = 0,
				LoadMapMenu,
				CreateMapMenu,
				Play,
				ChoosePlayModeMenu
			};
			Mode mode;
			std::string argument;

			PlayMode(Mode _mode, std::string _argument = "")
				: mode(_mode)
				, argument(_argument)
			{}

			PlayMode() = delete;

		};

		struct BoundingUpdate : Message < BoundingUpdate >
		{
			BoundingUpdate() = delete;
			BoundingUpdate(const Rect &_rect)
				: rect(_rect)
			{

			}
			Rect rect;
		};


	}
}