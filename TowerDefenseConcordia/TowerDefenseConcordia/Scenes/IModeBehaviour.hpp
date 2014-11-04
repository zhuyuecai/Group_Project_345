#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../RectArea.hpp"

namespace TDC
{
	class Scene : public RectArea
	{
	public:
		Scene()
			: RectArea()
		{}
		virtual void init() = 0;
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow) = 0;
		virtual ~Scene()
		{}
	};
}