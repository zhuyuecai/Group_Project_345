#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../RectArea.hpp"

namespace TDC
{
	class Game;
	class Scene : public RectArea
	{
	public:
		Scene()
			: RectArea()
		{}
		virtual void init(){};
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow) = 0;
		virtual ~Scene()
		{}
		inline void setGamePtr(Game *ptr) { _game = ptr; }
	protected:
		Game *_game;
	};
}