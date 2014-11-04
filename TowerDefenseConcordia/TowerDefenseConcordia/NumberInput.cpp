#include "NumberInput.hpp"

namespace TDC
{
	NumberInput::NumberInput(sf::Vector2f position
		, sf::Vector2f dimensions
		, const sf::Color &textColor
		, const sf::Color &bgColor
		, std::size_t fontSize)
		: TextInput(position, dimensions, "0", textColor, bgColor, fontSize)
		, _min(5)
		, _max(30)
	{
		_text.setString(std::to_string(_min));
	}

	void NumberInput::_event(const sf::Event &event)
	{
		auto copy = _text.getString();
		TextInput::_event(event);
		if (copy != _text.getString())
		{
			auto str = std::string(_text.getString());
			while (str.size() > 0 && (str.back() < '0' || str.back() > '9'))
			{
				str.pop_back();
			}
			_text.setString(str);
			if (str.size() == 0)
				return;
			_resized();
		}
		if (!_focus)
		{
			if (_text.getString().getSize() > 8)
				_text.setString(std::to_string(_max));
			auto v = getInt();
			if (v < _min)
				_text.setString(std::to_string(_min));
			else if (v > _max)
				_text.setString(std::to_string(_max));
			_resized();
		}
	}
}