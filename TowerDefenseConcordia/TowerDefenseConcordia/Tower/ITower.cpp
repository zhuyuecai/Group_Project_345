#include "ITower.hpp"
#include "../Critter.hpp"
#include <algorithm>
#include <list>
#include <functional>

namespace TDC
{
	namespace TowerType
	{
		const std::size_t IType::maxLevel = 5;

		const std::array<std::size_t, 5> Type<MiniGun>::price = { 10, 12, 14, 16, 18 };
		const std::array<float, 5> Type<MiniGun>::power = { 0.5f, 0.8, 1.2, 1.4, 2 };
		const std::array<sf::Color, 5> Type<MiniGun>::color = {
			sf::Color(0, 65, 230)
			, sf::Color(0, 119, 230)
			, sf::Color(0, 173, 230)
			, sf::Color(0, 230, 216)
			, sf::Color(0, 230, 176) };
		const std::array<std::size_t, 5> Type<MiniGun>::refund = { 5, 6, 7, 8, 9 };
		const std::array<std::size_t, 5> Type<MiniGun>::range = { 2, 2, 3, 3, 4 };
		const std::array<float, 5> Type<MiniGun>::fireRate = { 0.13f, 0.125f, 0.12f, 0.115f, 0.110f };
		Critter *Type<MiniGun>::shoot(std::vector<Critter*> &critters, const sf::Vector2f &center, float &fireCounter, float fireRate)
		{
			if (fireCounter < fireRate || critters.empty())
				return nullptr;
			fireCounter = 0;

			std::sort(critters.begin(), critters.end(), [&](Critter *&a, Critter *&b)
			{
				float aa = sqrt((a->getPosition().x - center.x)*(a->getPosition().x - center.x) + (a->getPosition().y - center.y)*(a->getPosition().y - center.y));
				float bb = sqrt((b->getPosition().x - center.x)*(b->getPosition().x - center.x) + (b->getPosition().y - center.y)*(b->getPosition().y - center.y));
				return aa < bb;
			});

			return critters.front();
		}


		const std::array<std::size_t, 5> Type<Rocket>::price = { 15, 20, 25, 30, 35 };
		const std::array<float, 5> Type<Rocket>::power = { 1.5f, 3.0f, 4.5f, 6.0f, 7.5f };
		const std::array<sf::Color, 5> Type<Rocket>::color = {
			sf::Color(230, 279, 0)
			, sf::Color(230, 146, 0)
			, sf::Color(230, 108, 0)
			, sf::Color(230, 65, 0)
			, sf::Color(230, 0, 0) };
		const std::array<std::size_t, 5> Type<Rocket>::refund = { 10, 15, 20, 25, 30 };
		const std::array<std::size_t, 5> Type<Rocket>::range = { 2, 3, 3, 4, 4 };
		const std::array<float, 5> Type<Rocket>::fireRate = { 1.5f, 1.25f, 1.0f, 0.75f, 0.50f };

		Critter *Type<Rocket>::shoot(std::vector<Critter*> &critters, const sf::Vector2f &center, float &fireCounter, float fireRate)
		{
			if (fireCounter < fireRate || critters.empty())
				return nullptr;
			fireCounter = 0;

			std::sort(std::begin(critters), std::end(critters), [center](const Critter *a, const Critter *b)
			{
				return a->getSpeed() > b->getSpeed();
			});

			return critters.front();
		}

		const std::array<std::size_t, 5> Type<Laser>::price = { 20, 25, 30, 35, 40 };
		const std::array<float, 5> Type<Laser>::power = { 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
		const std::array<sf::Color, 5> Type<Laser>::color = {
			sf::Color(20, 279, 0)
			, sf::Color(20, 146, 30)
			, sf::Color(20, 108, 60)
			, sf::Color(20, 65, 90)
			, sf::Color(20, 0, 120) };
		const std::array<std::size_t, 5> Type<Laser>::refund = { 10, 15, 20, 25, 30 };
		const std::array<std::size_t, 5> Type<Laser>::range = { 2, 2, 2, 3, 4 };
		const std::array<float, 5> Type<Laser>::fireRate = { 1.5f, 1.25f, 1.0f, 0.75f, 0.50f };

		Critter *Type<Laser>::shoot(std::vector<Critter*> &critters, const sf::Vector2f &center, float &fireCounter, float fireRate)
		{
			if (fireCounter < fireRate || critters.empty())
				return nullptr;
			fireCounter = 0;

			std::sort(std::begin(critters), std::end(critters), [center](const Critter *a, const Critter *b)
			{
				return a->getLife() > b->getLife();
			});

			return critters.front();
		}
	}
}