#pragma once 

#include "../WindowManager.hpp"
#include "../Map.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MapMessages.hpp"
#include "../Critter.hpp"
#include "IModeBehaviour.hpp"
#include <memory>

namespace TDC
{
	class EditMapScene : public Scene
	{
	public:
		EditMapScene()
		{}

		virtual ~EditMapScene()
		{}

		virtual void init();
		virtual void update(const sf::Time &dt, sf::RenderWindow *window);
	private:
		std::vector < CellType > _mapData;
	};
}