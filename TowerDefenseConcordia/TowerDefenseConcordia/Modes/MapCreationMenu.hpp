#pragma once

#include "IModeBehaviour.hpp"
#include "../MapMessages.hpp"
#include <iostream>
#include "../TextInput.hpp"
#include "../NumberInput.hpp"

namespace TDC
{
	class MapCreationMenuBehaviour : public IModeBehaviour
	{
	public:
		virtual void init();
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow);
		virtual ~MapCreationMenuBehaviour();
	private:
		std::unique_ptr<TextInput> _mapName;
		std::unique_ptr<NumberInput> _mapWidth;
		std::unique_ptr<NumberInput> _mapHeight;
	};
}