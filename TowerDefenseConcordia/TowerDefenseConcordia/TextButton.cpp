#include "TextButton.hpp"

namespace TDC
{
	std::unique_ptr<sf::Font> TextButton::_font = nullptr;
	std::once_flag TextButton::_flag;

	TextButton::TextButton(sf::Vector2u position
		, sf::Vector2u dimensions
		, const std::string &text
		, const sf::Color &textColor
		, const sf::Color &bgColor
		, std::size_t fontSize)
		: IButton(position, dimensions)
	{
		std::call_once(_flag, [&](){
			_font = std::make_unique<sf::Font>();
			assert(_font->loadFromFile("../assets/arial.ttf"));
		});
		_computeBoundingBox();
		_shape.setPosition((float)_bbX.x, (float)_bbY.x);
		_shape.setSize(sf::Vector2f((float)(_bbX.y - _bbX.x), (float)(_bbY.y - _bbY.x)));
		_shape.setFillColor(bgColor);
		_text.setFont(*_font);
		_text.setCharacterSize(fontSize);
		_text.setPosition((float)_bbX.x, (float)_bbY.y);
		_text.setColor(textColor);
		_text.setString(text);
	}

	void TextButton::_update(const sf::Time &dt, sf::RenderWindow *window)
	{
		if (window && _active)
		{
			window->draw(_shape);
			window->draw(_text);
		}
	}
	void TextButton::_onClick()
	{
		if (_onClickCallback)
			_onClickCallback();
	}

	void TextButton::_event(const sf::Event &event)
	{
	}

	void TextButton::_init()
	{}

	void TextButton::_resized()
	{
		_shape.setPosition((float)_bbX.x, (float)_bbY.x);
		_shape.setSize(sf::Vector2f((float)_bbX.y - _bbX.x, (float)_bbY.y - _bbY.x));
		_text.setPosition((float)_bbX.x + (_bbX.y - _bbX.x) / 2.0f - _text.getLocalBounds().width / 2.0f, (float)_bbY.x + (_bbY.y - _bbY.x) / 2.0f - _text.getLocalBounds().height / 2.0f);
	}
}