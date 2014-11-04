#pragma once

#include "RectArea.hpp"
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/Font.hpp>
#include <mutex>
#include <assert.h>
#include <memory.h>

namespace TDC
{
	class TextButton : public RectArea
	{
	public:
		TextButton(sf::Vector2f position = sf::Vector2f(0, 0) /*percent*/
			, sf::Vector2f dimensions = sf::Vector2f(0, 0) /*percent*/
			, const std::string &text = "Text"
			, const sf::Color &textColor = sf::Color(255, 255, 255, 255)
			, const sf::Color &bgColor = sf::Color(122, 122, 122, 255)
			, std::size_t fontSize = 20);
		virtual ~TextButton(){}
	protected:
		virtual void _update(const sf::Time &dt, sf::RenderWindow *window);
		virtual void _onClick();
		virtual void _resized();

		sf::RectangleShape _shape;
		sf::Text _text;
		static std::unique_ptr<sf::Font> _font;
		static std::once_flag _flag;
	};
}