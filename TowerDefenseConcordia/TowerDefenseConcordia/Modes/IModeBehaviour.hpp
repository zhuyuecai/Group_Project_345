#pragma once

#include "../ObserverPattern.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace TDC
{
	class IModeBehaviour : public PubSub
	{
	public:
		virtual void init() = 0;
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow) = 0;
		virtual ~IModeBehaviour()
		{}
	};
}