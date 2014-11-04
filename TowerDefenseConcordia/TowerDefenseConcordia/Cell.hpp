#pragma once

#include <cstddef>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

namespace TDC
{
	class Map;

	enum CellType : bool
	{
		Wall = 0
		, Path
	};

#define INVALID ((std::size_t)(-1))

	class Cell
	{
	private:
		friend class Map;

		CellType _type;
		std::size_t _index;
		std::size_t _next;
		unsigned char _dir;

		// private methods, only map can access them
		inline void setNext(std::size_t n) { _next = n; }
		inline void setIndex(std::size_t i) { _index = i; }
		inline void setType(const CellType &t) { _type = t; }
	public:
		Cell(); // ctor
		~Cell(); // dtor

		// copy and move operators
		Cell(const Cell &o);
		Cell(Cell &&o);
		Cell &operator=(const Cell &o);
		Cell &operator=(Cell &&o);

		// type getter
		inline const CellType &getType() const { return _type; }

		// type checker
		inline bool isType(const CellType &t) const { return t == _type; }

		// index getter
		inline std::size_t getIndex() const { return _index; }

		// return true if cell is connected to another
		inline bool hasNext() const { return std::size_t(-1) != _next; }

		// return next cell index or std::size_t(-1) if not connected
		inline std::size_t getNext() const { return _next; }

		// serialization function
		template < typename Archive >
		void serialize(Archive &ar)
		{
			ar(cereal::make_nvp("Type", _type)
				, cereal::make_nvp("Index", _index)
				, cereal::make_nvp("Next", _next));
		}
	};
}