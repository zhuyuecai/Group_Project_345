#pragma once

#include <array>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <cassert>

namespace TDC
{
	namespace TowerType
	{
		struct IType
		{
			virtual std::size_t getPrice(std::size_t _level) = 0;
			virtual std::size_t getPower(std::size_t _level) = 0;
			virtual const sf::Color &getColor(std::size_t _level) = 0;
			static const std::size_t maxLevel;
		};

		template <typename T>
		struct Type : public IType
		{
			virtual std::size_t getPrice(std::size_t _level) { return price[_level]; }
			virtual std::size_t getPower(std::size_t _level) { return power[_level]; }
			virtual const sf::Color &getColor(std::size_t _level) { return color[_level]; }

			static const std::array<std::size_t, 5> price;
			static const std::array<float, 5> power;
			static const std::array<sf::Color, 5> color;
		};

		struct MiniGun : public Type< MiniGun >
		{
		};

		struct Rocket : public Type < Rocket >
		{
		};
	}

	class ITower
	{
	public:
		virtual ~ITower(){}
		inline std::size_t getLevel() const { return _level; }
		inline void upgradeLevel() { if (_level < _properties->maxLevel) ++_level; }
		inline bool isUpgradable() const { return _level < _properties->maxLevel; }
		inline std::size_t getPrice() const {
			return _properties->getPrice(_level);
		}
		inline std::size_t getPower() const {
			return _properties->getPower(_level);
		}
		inline const sf::Color &getColor() const {
			return _properties->getColor(_level);
		}
		inline std::size_t getCellIndex() const { return _cellIndex; }
		inline void setCellIndex(std::size_t i) { _cellIndex = i; }
	protected:
		ITower(TowerType::IType* t)
			: _cellIndex(-1)
			, _level(0)
			, _properties(t)
		{}
		std::size_t _cellIndex;
		std::size_t _level;
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