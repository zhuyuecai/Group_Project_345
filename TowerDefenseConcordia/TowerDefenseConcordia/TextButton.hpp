#pragma once

#include "IButton.hpp"
#include <sfml/Graphics/RectangleShape.hpp>
#include <sfml/Graphics/Text.hpp>
#include <sfml/Graphics/Font.hpp>
#include <mutex>
#include <assert.h>
#include <memory.h>

namespace TDC
{
	class TextButton : public IButton
	{
	public:
		TextButton(sf::Vector2u position = sf::Vector2u(0, 0) /*percent*/
			, sf::Vector2u dimensions = sf::Vector2u(0, 0) /*percent*/
			, const std::string &text = "Text"
			, const sf::Color &textColor = sf::Color(255, 255, 255, 255)
			, const sf::Color &bgColor = sf::Color(122, 122, 122, 255)
			, std::size_t fontSize = 20);
	protected:
		virtual void _update(const sf::Time &dt, sf::RenderWindow *window);
		virtual void _onClick();
		virtual void _event(const sf::Event &event);
		virtual void _init();
		virtual void _resized();
	private:
		sf::RectangleShape _shape;
		sf::Text _text;
		static std::unique_ptr<sf::Font> _font;
		static std::once_flag _flag;
	};
}