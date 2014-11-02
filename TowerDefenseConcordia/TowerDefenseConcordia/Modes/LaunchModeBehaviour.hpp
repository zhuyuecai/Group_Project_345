#pragma once

#include "IModeBehaviour.hpp"
#include "../MapMessages.hpp"
#include <iostream>
#include "../TextButton.hpp"

namespace TDC
{
	class LaunchModeBehaviour : public IModeBehaviour
	{
	public:
		virtual void init();
		virtual void update(const sf::Time &dt, sf::RenderWindow *renderWindow);
		virtual ~LaunchModeBehaviour();
	private:
		std::unique_ptr<TextButton> _play;
		std::unique_ptr<TextButton> _edit;
	};
}