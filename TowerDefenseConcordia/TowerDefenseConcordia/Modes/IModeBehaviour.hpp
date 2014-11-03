#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../IButton.hpp"

namespace TDC
{
	class IModeBehaviour : public RectArea
	{
	public:
		IModeBehaviour()
			: RectArea()
		{}
		virtual void init() = 0;
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow) = 0;
		virtual ~IModeBehaviour()
		{}
	};
}