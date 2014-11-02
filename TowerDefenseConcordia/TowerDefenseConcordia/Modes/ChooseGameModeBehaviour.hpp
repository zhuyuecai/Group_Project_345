#pragma once

#include "IModeBehaviour.hpp"
#include "../MapMessages.hpp"
#include <iostream>
#include "../TextButton.hpp"

namespace TDC
{
	class ChooseGameModeBehaviour : public IModeBehaviour
	{
	public:
		virtual void init();
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow);
		virtual ~ChooseGameModeBehaviour();
	private:
		std::unique_ptr<TextButton> _randomMap;
		std::unique_ptr<TextButton> _loadMap;
	};
}