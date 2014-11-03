#pragma once

#include "TextButton.hpp"
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/Font.hpp>
#include <mutex>
#include <assert.h>
#include <memory.h>

namespace TDC
{
	class TextInput : public TextButton
	{
	public:
		TextInput(sf::Vector2u position = sf::Vector2u(0, 0) /*percent*/
			, sf::Vector2u dimensions = sf::Vector2u(0, 0) /*percent*/
			, const std::string &text = "Text"
			, const sf::Color &textColor = sf::Color(255, 255, 255, 255)
			, const sf::Color &bgColor = sf::Color(122, 122, 122, 255)
			, std::size_t fontSize = 20);
		std::string getValue() const { return _text.getString(); }
		virtual ~TextInput(){}
	protected:
		virtual void _event(const sf::Event &event);

		bool _focus;
	};
}