#include "ITower.hpp"

namespace TDC
{
	namespace TowerType
	{
		const std::size_t IType::maxLevel = 4;

		const std::array<std::size_t, 5> Type<MiniGun>::price = { 10, 12, 14, 16, 18 };
		const std::array<float, 5> Type<MiniGun>::power = { 1, 2, 3, 4, 5 };
		const std::array<sf::Color, 5> Type<MiniGun>::color = {
			sf::Color(0, 65, 230)
			, sf::Color(0, 119, 230)
			, sf::Color(0, 173, 230)
			, sf::Color(0, 230, 216)
			, sf::Color(0, 230, 176) };
		const std::array<std::size_t, 5> Type<MiniGun>::refund = { 5, 6, 7, 8, 9 };
		const std::array<std::size_t, 5> Type<MiniGun>::range = { 1, 1, 2, 2, 3 };
		const std::array<float, 5> Type<MiniGun>::fireRate = { 0.3f, 0.25f, 0.2f, 0.15f, 0.10f };


		const std::array<std::size_t, 5> Type<Rocket>::price = { 15, 20, 25, 30, 35 };
		const std::array<float, 5> Type<Rocket>::power = { 1.5f, 3.0f, 4.5f, 6.0f, 7.5f };
		const std::array<sf::Color, 5> Type<Rocket>::color = {
			sf::Color(230, 279, 0)
			, sf::Color(230, 146, 0)
			, sf::Color(230, 108, 0)
			, sf::Color(230, 65, 0)
			, sf::Color(230, 0, 0) };
		const std::array<std::size_t, 5> Type<Rocket>::refund = { 10, 15, 20, 25, 30 };
		const std::array<std::size_t, 5> Type<Rocket>::range = { 1, 2, 3, 4, 5 };
		const std::array<float, 5> Type<Rocket>::fireRate = { 1.5f, 1.25f, 1.0f, 0.75f, 0.50f };
	}
}