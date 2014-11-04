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