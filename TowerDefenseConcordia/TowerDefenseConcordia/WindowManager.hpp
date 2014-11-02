#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace TDC
{
	class WindowManager
	{
	public:
		WindowManager();

		virtual ~WindowManager();

		virtual void init();

		void setSize(const sf::Vector2u &size);

		const sf::Vector2u &getSize() const;

		void launch();

	protected:
		virtual void _updateEvents(const sf::Event &event) = 0;
		virtual void _update(const sf::Time dt) = 0;
		sf::RenderWindow _window;
	};
}