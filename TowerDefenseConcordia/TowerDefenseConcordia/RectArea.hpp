#pragma once

#include <sfml/System/Vector2.hpp>
#include "ObserverPattern.hpp"
#include <sfml/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "MapMessages.hpp"

namespace TDC
{
	class RectArea : public PubSub
	{
	public:
		RectArea(sf::Vector2f position = sf::Vector2f(0, 0) /*percent*/
			, sf::Vector2f dimensions = sf::Vector2f(100, 100) /*percent*/)
			: _percent(Rect(position.x, position.y, dimensions.x, dimensions.y))
			, _hasParent(false)
			, _root(false)
			, _centered(true)
		{
			_setup();
		}

		RectArea(float percentPosX, float percentPosY, float percentWidth, float percentHeight)
			: _percent(Rect(percentPosX, percentPosY, percentWidth, percentHeight))
			, _hasParent(false)
			, _root(false)
			, _centered(true)
		{
			_setup();
		}
   

		virtual ~RectArea(){}

		void setParent(RectArea *parent)
		{
			parent->addSubscriber(this->getHandle());
			_parentBox = parent->_pixels;
			_updateBBox();
		}

		inline void setPosition(const sf::Vector2f &position)
		{
			_percent.left = position.x;
			_percent.top = position.y;
			_updateBBox();
		}

		inline void setDimension(const sf::Vector2f &dimension)
		{
			_percent.width = dimension.x;
			_percent.height = dimension.y;
			_updateBBox();
		}


		void update(const sf::Time &dt, sf::RenderWindow *window)
		{
			_update(dt, window);
		}

		bool isPointIn(float x, float y)
		{
			return _pixels.contains(x, y);
		}

		inline void setOnClickCallback(const std::function<bool()> &fn)
		{
			_onClickCallback = fn;
		}

		inline void setOnHoverCallback(const std::function<void()> &fn)
		{
			_onHoverCallback = fn;
		}

		void setRootArea(float pxWidth, float pxHeight)
		{
			_percent.top = 50;
			_percent.left = 50;
			_percent.width = 100;
			_percent.height = 100;
			_parentBox = { 0, 0, pxWidth, pxHeight };
			_root = true;
			_updateBBox();
		}

		inline void setCentered(bool tof)
		{
			_centered = tof;
			_updateBBox();
		}

	protected:
		virtual void _update(const sf::Time &dt, sf::RenderWindow *window){};
		virtual bool _onClick(){ return true; };
		virtual void _onHover(){};
		virtual bool _event(const sf::Event &event){ return true; };
		virtual void _init(){};
		virtual void _resized(){};

		void _updateBBox()
		{
			_computeBBox();
			_resized();
			this->publish<Msg::BoundingUpdate>(_pixels);
		}

		void _computeBBox()
		{
			if (_root)
			{
				_pixels = _parentBox;
				return;
			}
			if (_centered)
			{
				float w = _parentBox.width * _percent.width / 100.0f;
				float h = _parentBox.height * _percent.height / 100.0f;

				_pixels.left = _parentBox.left + (_parentBox.width * _percent.left / 100.0f) - (w / 2.0f);
				_pixels.width = w;
				_pixels.top = _parentBox.top + (_parentBox.height * _percent.top / 100.0f) - (h / 2.0f);
				_pixels.height = h;
			}
			else
			{
				float w = _parentBox.width * _percent.width / 100.0f;
				float h = _parentBox.height * _percent.height / 100.0f;

				_pixels.left = _parentBox.left + (_parentBox.width * _percent.left / 100.0f);
				_pixels.width = w;
				_pixels.top = _parentBox.top + (_parentBox.height * _percent.top / 100.0f);
				_pixels.height = h;
			}
		}

		std::function<bool()> _onClickCallback;
		std::function<void()> _onHoverCallback;
		bool _hasParent;
		Rect _pixels;
		Rect _percent;
		Rect _parentBox;
		bool _root;
		bool _centered;
private:
	void _setup()
	{
		this->subcribeToMessage<Msg::BoundingUpdate>([&](const IMessage *msg)
		{
			auto *m = static_cast<const Msg::BoundingUpdate*>(msg);
			_parentBox = m->rect;
			_updateBBox();
		});

		this->subcribeToMessage<Msg::Event>([&](IMessage *msg)
		{
			auto *m = static_cast <Msg::Event*>(msg);
			if (m->event.type == sf::Event::MouseButtonReleased
				&& isPointIn(m->event.mouseButton.x, m->event.mouseButton.y))
			{
				if (!_onClick())
					return;
				if (_onClickCallback)
					if (!_onClickCallback())
						return;
			}
			else if (m->event.type == sf::Event::MouseMoved
				&& isPointIn(m->event.mouseMove.x, m->event.mouseMove.y))
			{
				_onHover();
				if (_onHoverCallback)
					_onHoverCallback();
			}

			if (_event(m->event))
				publish<Msg::Event>(m->event);
		});

		_init();
	}
	};
}