#include "WindowManager.hpp"

namespace TDC
{
	WindowManager::WindowManager()
	{}

	WindowManager::~WindowManager()
	{}

	void WindowManager::init()
	{
		_window.create(sf::VideoMode(800, 600), "Concordia Tower Defense");
		_window.setFramerateLimit(60);
	}

	void WindowManager::setSize(const sf::Vector2u &size)
	{
		_window.setSize(size);
	}

	const sf::Vector2u &WindowManager::getSize() const
	{
		return _window.getSize();
	}

	void WindowManager::launch()
	{
		sf::Clock clock;
		while (_window.isOpen())
		{
			sf::Time elapsed = clock.restart();
			sf::Event windowEvent;

			_window.clear(sf::Color::Black);
			_update(elapsed);

			while (_window.pollEvent(windowEvent))
			{
				_updateEvents(windowEvent);
				if (windowEvent.type == sf::Event::Closed)
				{
					_window.close();
				}
			}
			_window.display();
		}
	}
}

