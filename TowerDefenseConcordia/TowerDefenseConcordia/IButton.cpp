//#include "IButton.hpp"
//
//namespace TDC
//{
//	RectArea::RectArea(sf::Vector2u position
//		, sf::Vector2u dimensions)
//		: _position(position)
//		, _dimensions(dimensions)
//		, _active(true)
//		, _dirty(true)
//	{
//	}
//
//	RectArea::~RectArea(){}
//
//
//	void RectArea::update(const sf::Time &dt, sf::RenderWindow *window)
//	{
//		if (!_active)
//			return;
//		_update(dt, window);
//	}
//
//	void RectArea::init()
//	{
//		subcribeToMessage<Msg::Resize>([&](IMessage *msg)
//		{
//			auto *m = static_cast <Msg::Resize*>(msg);
//			_windowSize = m->size;
//			_dirty = true;
//
//		});
//
//		subcribeToMessage<Msg::Event>([&](IMessage *msg)
//		{
//			auto *m = static_cast <Msg::Event*>(msg);
//			if (m->event.type == sf::Event::MouseButtonReleased
//				&& isPointIn(m->event.mouseButton.x, m->event.mouseButton.y))
//			{
//				_onClick();
//			}
//			_event(m->event);
//		});
//		_init();
//	}
//
//	void RectArea::_computeBoundingBox()
//	{
//		auto size = _windowSize;
//		float w = size.x * (float)_dimensions.x / 100.0f;
//		float h = size.y * (float)_dimensions.y / 100.0f;
//
//		_bbX.x = (unsigned int)((size.x * (float)_position.x / 100.0f) - (w / 2.0f));
//		_bbX.y = (unsigned int)(_bbX.x + w);
//		_bbY.x = (unsigned int)((size.y * (float)_position.y / 100.0f) - (h / 2.0f));
//		_bbY.y = (unsigned int)(_bbY.x + h);
//		_dirty = false;
//		_resized();
//	}
//
//}