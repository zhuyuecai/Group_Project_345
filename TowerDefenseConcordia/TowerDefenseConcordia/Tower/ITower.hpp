#pragma once

#include <array>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <cassert>
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace TDC
{
	class Critter;
	namespace TowerType
	{
		struct IType
		{
			virtual std::size_t getPrice(std::size_t _level) = 0;
			virtual float getPower(std::size_t _level) = 0;
			virtual const sf::Color &getColor(std::size_t _level) = 0;
			virtual const std::size_t getRefund(std::size_t _level) = 0;
			virtual const std::size_t getRange(std::size_t _level) = 0;
			virtual const float getFireRate(std::size_t _level) = 0;
			virtual Critter *shoot(std::vector<Critter*> &critters, const sf::Vector2f &center, float &fireCounter, float _fireRate) = 0;
			static const std::size_t maxLevel;
		};

		template <typename T>
		struct Type : public IType
		{
			virtual std::size_t getPrice(std::size_t _level) { return price[_level]; }
			virtual float getPower(std::size_t _level) { return power[_level]; }
			virtual const sf::Color &getColor(std::size_t _level) { return color[_level]; }
			virtual const std::size_t getRefund(std::size_t _level) { return refund[_level]; }
			virtual const std::size_t getRange(std::size_t _level) { return range[_level]; }
			virtual const float getFireRate(std::size_t _level) { return fireRate[_level]; }
			virtual Critter *shoot(std::vector<Critter*> &critters, const sf::Vector2f &center, float &fireCounter, float _fireRate);

			static std::size_t getCreationPrice() { return price[0]; }

			static const std::array<std::size_t, 5> price;
			static const std::array<float, 5> power;
			static const std::array<sf::Color, 5> color;
			static const std::array<std::size_t, 5> refund;
			static const std::array<std::size_t, 5> range;
			static const std::array<float, 5> fireRate;
		};

		struct MiniGun : public Type< MiniGun >
		{
		};

		struct Rocket : public Type < Rocket >
		{
		};

		struct Laser : public Type < Laser >
		{
		};
	}

	class ITower
	{
	public:
		virtual ~ITower(){}
		inline std::size_t getLevel() const { return _level; }
		inline void upgradeLevel() { if (_level < _properties->maxLevel) ++_level; }
		inline bool isUpgradable() const { return _level < _properties->maxLevel - 1; }
		inline std::size_t getPrice() const {
			return _properties->getPrice(_level);
		}
		inline float getPower() const {
			return _properties->getPower(_level);
		}
		inline const sf::Color &getColor() const {
			return _properties->getColor(_level);
		}
		inline std::size_t getRefund() const {
			return _properties->getRefund(_level);
		}
		inline std::size_t getRange() const {
			return _properties->getRange(_level);
		}
		inline float getFireRate() const {
			return _properties->getFireRate(_level);
		}
		inline std::size_t getNextPrice() const {
			return _properties->getPrice(_level + 1);
		}
		inline std::size_t getCellIndex() const { return _cellIndex; }
		inline void setCellIndex(std::size_t i) { _cellIndex = i; }
		inline void updateFireCounter(float t) { _fireTimer += t; }

		Critter *shoot(const sf::Vector2f &position)
		{
			return _properties->shoot(_critters, position, _fireTimer, getFireRate());
		}
		std::vector<Critter*> _critters;
	protected:
		ITower(TowerType::IType* t)
			: _cellIndex(-1)
			, _level(0)
			, _properties(t)
			, _fireTimer(0.0f)
		{}
		std::size_t _cellIndex;
		std::size_t _level;
		float _fireTimer;
		const std::unique_ptr < TowerType::IType > _properties;
	};

	template < typename T >
	class Tower : public ITower, private T
	{
	public:
		Tower()
			: ITower(new T())
		{
			assert(_properties.get() != nullptr);
		}
	private:
	};
}