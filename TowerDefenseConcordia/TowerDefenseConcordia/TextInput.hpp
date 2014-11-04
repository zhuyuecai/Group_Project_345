#pragma once

#include "TextButton.hpp"

namespace TDC
{
	class TextInput : public TextButton
	{
	public:
		TextInput(sf::Vector2f position = sf::Vector2f(0, 0) /*percent*/
			, sf::Vector2f dimensions = sf::Vector2f(0, 0) /*percent*/
			, const std::string &text = "Text"
			, const sf::Color &textColor = sf::Color(255, 255, 255, 255)
			, const sf::Color &bgColor = sf::Color(122, 122, 122, 255)
			, std::size_t fontSize = 20);
		inline std::string getValue() const { return _text.getString(); }
		virtual ~TextInput(){}
	protected:
		virtual void _event(const sf::Event &event);

		bool _focus;
	};
}