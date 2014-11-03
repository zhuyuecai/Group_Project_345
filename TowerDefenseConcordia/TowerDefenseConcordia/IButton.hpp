#pragma once

#include <sfml/System/Vector2.hpp>
#include "ObserverPattern.hpp"
#include <sfml/Graphics/RenderWindow.hpp>
#include "MapMessages.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace TDC
{
	typedef sf::Rect<float> Rect;

	class RectArea : public PubSub
	{
	public:

		struct BoundingUpdate : Message < BoundingUpdate >
		{
			BoundingUpdate() = delete;
			BoundingUpdate(const Rect &_rect)
				: rect(_rect)
			{

			}
			Rect rect;
		};

		RectArea(sf::Vector2u position = sf::Vector2u(0, 0) /*percent*/
			, sf::Vector2u dimensions = sf::Vector2u(100, 100) /*percent*/)
			: _percent(Rect(position.x, position.y, dimensions.x, dimensions.y))
			, _hover(false)
			, _focus(false)
			, _hasParent(false)
		{
			_setup();
		}

		RectArea(float percentPosX, float percentPosY, float percentWidth, float percentHeight)
			: _percent(Rect(percentPosX, percentPosY, percentWidth, percentHeight))
			, _hover(false)
			, _focus(false)
			, _hasParent(false)
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

		inline void setPosition(const sf::Vector2u &position)
		{
			_percent.left = position.x;
			_percent.top = position.y;
			_updateBBox();
		}

		inline void setDimension(const sf::Vector2u &dimension)
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

		inline void setOnClickCallback(const std::function<void()> &fn)
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
			_updateBBox();
		}

	protected:
		virtual void _update(const sf::Time &dt, sf::RenderWindow *window){};
		virtual void _onClick(){};
		virtual void _onHover(){};
		virtual void _event(const sf::Event &event){};
		virtual void _init(){};
		virtual void _resized(){};

		void _updateBBox()
		{
			_computeBBox();
			_resized();
			this->publish<BoundingUpdate>(_pixels);
		}

		void _computeBBox()
		{
			float w = _parentBox.width * _percent.width / 100.0f;
			float h = _parentBox.height * _percent.height / 100.0f;

			_pixels.left = (_parentBox.left * _percent.left / 100.0f) - (w / 2.0f);
			_pixels.width = w;
			_pixels.top = (_parentBox.top * _percent.top / 100.0f) - (h / 2.0f);
			_pixels.height = h;
		}

		std::function<void()> _onClickCallback;
		std::function<void()> _onHoverCallback;
		bool _hover;
		bool _focus;
		bool _hasParent;
		Rect _pixels;
		Rect _percent;
		Rect _parentBox;
private:
	void _setup()
	{
		this->subcribeToMessage<BoundingUpdate>([&](const IMessage *msg)
		{
			auto *m = static_cast<const BoundingUpdate*>(msg);
			_parentBox = m->rect;
			_updateBBox();
		});

		this->subcribeToMessage<Msg::Event>([&](IMessage *msg)
		{
			auto *m = static_cast <Msg::Event*>(msg);
			if (m->event.type == sf::Event::MouseButtonReleased
				&& isPointIn(m->event.mouseButton.x, m->event.mouseButton.y))
			{
				_onClick();
				if (_onClickCallback)
					_onClickCallback();
			}
			else if (m->event.type == sf::Event::MouseMoved
				&& isPointIn(m->event.mouseMove.x, m->event.mouseMove.y))
			{
				_onHover();
				if (_onHoverCallback)
					_onHoverCallback();
			}

			_event(m->event);
			publish<Msg::Event>(m->event);
		});

		_init();
	}
	};
}