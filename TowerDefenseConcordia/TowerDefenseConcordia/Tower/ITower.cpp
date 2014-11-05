#include "ITower.hpp"

namespace TDC
{
	namespace TowerType
	{
		const std::size_t IType::maxLevel = 4;

		const std::array<std::size_t, 5> Type<MiniGun>::price = { 10, 12, 14, 16, 18 };
		const std::array<float, 5> Type<MiniGun>::power = { 1, 2, 3, 4, 5 };
		const std::array<sf::Color, 5> Type<MiniGun>::color = { sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue, sf::Color::Blue };

		const std::array<std::size_t, 5> Type<Rocket>::price = { 15, 20, 25, 30, 35 };
		const std::array<float, 5> Type<Rocket>::power = { 1.5f, 3.0f, 4.5f, 6.0f, 7.5f };
		const std::array<sf::Color, 5> Type<Rocket>::color = { sf::Color::Red, sf::Color::Red, sf::Color::Red, sf::Color::Red, sf::Color::Red };
	}
}