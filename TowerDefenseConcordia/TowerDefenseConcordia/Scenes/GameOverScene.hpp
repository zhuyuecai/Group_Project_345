#pragma once

#include "IModeBehaviour.hpp"
#include "../MapMessages.hpp"
#include <iostream>
#include "../TextButton.hpp"

namespace TDC
{
	class GameOver : public Scene
	{
	public:
		virtual void init();
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow);
		virtual ~GameOver();
		void setScore(int s);
	private:
		std::unique_ptr<TextButton> _text;
	};
}