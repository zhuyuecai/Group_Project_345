#include "TextButton.hpp"

namespace TDC
{
	std::unique_ptr<sf::Font> TextButton::_font = nullptr;
	std::once_flag TextButton::_flag;

	TextButton::TextButton(sf::Vector2f position
		, sf::Vector2f dimensions
		, const std::string &text
		, const sf::Color &textColor
		, const sf::Color &bgColor
		, std::size_t fontSize)
		: RectArea(position, dimensions)
	{
		std::call_once(_flag, [&](){
			_font = std::make_unique<sf::Font>();
			assert(_font->loadFromFile("../assets/arial.ttf"));
		});

		_shape.setPosition(_pixels.left, _pixels.top);
		_shape.setSize({ _pixels.width, _pixels.height });
		_shape.setFillColor(bgColor);
		_text.setFont(*_font);
		_text.setCharacterSize(fontSize);
		_text.setPosition(_pixels.left, _pixels.top);
		_text.setColor(textColor);
		_text.setString(text);
	}

	void TextButton::_update(const sf::Time &dt, sf::RenderWindow *window)
	{
		if (window && _show)
		{
			window->draw(_shape);
			window->draw(_text);
		}
	}

	void TextButton::_resized()
	{
		_shape.setPosition({ _pixels.left, _pixels.top });
		_shape.setSize(sf::Vector2f(_pixels.width, _pixels.height));
		if (_centered)
		{
			_text.setPosition(_pixels.left + (_pixels.width) / 2.0f - _text.getLocalBounds().width / 2.0f, _pixels.top + (_pixels.height) / 2.0f - _text.getLocalBounds().height / 2.0f);
		}
		else
		{
			_text.setPosition(_pixels.left + (_pixels.width) / 30.0f, _pixels.top + (_pixels.height) / 30.0f);
		}
		if (_text.getLocalBounds().width >= _pixels.width && _pixels.width > 0)
		{
			float scale = _pixels.width / (_text.getLocalBounds().width + 5.0f);
			_text.scale({ scale, 1.0f });
		}
	}
}