#pragma once

#include <sfml/System/Vector2.hpp>
#include "ObserverPattern.hpp"
#include <sfml/Graphics/RenderWindow.hpp>
#include "MapMessages.hpp"

namespace TDC
{
	// Button interface
	class IButton : public PubSub
	{
	public:
		IButton(sf::Vector2u position = sf::Vector2u(0, 0) /*percent*/
			, sf::Vector2u dimensions = sf::Vector2u(0, 0) /*percent*/);
	
		virtual ~IButton();

		inline void setPosition(const sf::Vector2u &position)
		{
			_position = position;
		}

		inline void setDimension(const sf::Vector2u &dimension)
		{
			_dimensions = dimension;
		}

		inline void setActive(bool tof)
		{
			_active = tof;
		}

		void update(const sf::Time &dt, sf::RenderWindow *window);
		void init();
		inline bool isPointIn(unsigned int x, unsigned int y)
		{
			if (_dirty)
			{				
				_computeBoundingBox();
			}
			return (x >= _bbX.x && x <= _bbX.y && y >= _bbY.x && y <= _bbY.y);
		}

		inline void setOnClickCallback(const std::function<void()> &fn)
		{
			_onClickCallback = fn;
		}
	protected:
		virtual void _update(const sf::Time &dt, sf::RenderWindow *window) = 0;
		virtual void _onClick() = 0;
		virtual void _event(const sf::Event &event) = 0;
		virtual void _init() = 0;
		virtual void _resized() = 0;

		void _computeBoundingBox();

		std::function<void()> _onClickCallback;
		sf::Vector2u _position;
		sf::Vector2u _dimensions;
		bool _active;
		bool _dirty;
		sf::Vector2u _bbX;
		sf::Vector2u _bbY;
		sf::Vector2u _windowSize;
	};
}