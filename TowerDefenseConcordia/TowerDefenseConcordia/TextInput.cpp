#include "TextInput.hpp"

namespace TDC
{
	TextInput::TextInput(sf::Vector2u position
		, sf::Vector2u dimensions
		, const std::string &text
		, const sf::Color &textColor
		, const sf::Color &bgColor
		, std::size_t fontSize)
		: TextButton(position, dimensions, text, textColor, bgColor, fontSize)
		, _focus(false)
	{
	}

	void TextInput::_event(const sf::Event &event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (this->isPointIn(event.mouseButton.x, event.mouseButton.y))
				_focus = true;
			else
				_focus = false;
		}
		else if (event.type == sf::Event::KeyReleased && _focus)
		{
			if (event.key.code >= sf::Keyboard::Key::A && event.key.code <= sf::Keyboard::Key::Z)
			{
				_text.setString(_text.getString() + (char)((unsigned short)('a') + event.key.code));
			}
			else if (event.key.code == sf::Keyboard::Key::BackSpace && _text.getString().getSize() > 0)
			{
				auto str = std::string(_text.getString());
				str = str.substr(0, str.size() - 1);
				_text.setString(str);
			}
			_computeBoundingBox();
		}
	}
}