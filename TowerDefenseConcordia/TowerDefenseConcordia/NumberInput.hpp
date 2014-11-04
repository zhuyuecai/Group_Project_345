#pragma once

#include "TextInput.hpp"

namespace TDC
{
	class NumberInput : public TextInput
	{
	public:
		NumberInput(sf::Vector2f position = sf::Vector2f(0, 0) /*percent*/
			, sf::Vector2f dimensions = sf::Vector2f(0, 0) /*percent*/
			, const sf::Color &textColor = sf::Color(255, 255, 255, 255)
			, const sf::Color &bgColor = sf::Color(122, 122, 122, 255)
			, std::size_t fontSize = 20);

		inline int getInt() const
		{
			if (_text.getString().getSize() == 0)
				return 0;
			return std::stoi(std::string(_text.getString()));
		}
		virtual ~NumberInput(){}
		inline void setRange(int min, int max) { _min = min; _max = max; }
	protected:
		virtual void _event(const sf::Event &event);
		int _min;
		int _max;
	};
}