#pragma once

#include "Cell.hpp"
#include <vector>
#include <cereal/types/vector.hpp>

#include "ObserverPattern.hpp"

namespace TDC
{

	enum PathGenerationOption : bool
	{
		Straight = 0
		, None = 1
	};

	class Map : public PubSub
	{
	private:
		std::vector<Cell> _array;
		std::size_t _width;
		std::size_t _height;
		std::size_t _start;
		std::size_t _end;
		std::size_t _cellRatio;

		// return true if map configuations are correct
		inline bool _valid() const { return (_width != INVALID && _height != INVALID && _start < _height && _end < _height); }

	public:
		Map(); // ctor
		~Map(); // dtor

		// Copy operators are canceled
		Map(const Map &o) = delete;
		Map(Map &&o) = delete;
		Map &operator=(const Map &o) = delete;
		Map &operator=(Map &&o) = delete;

		inline void setCellSizeRatio(std::size_t r) { _cellRatio = r; }

		// return cell ptr based on x/y coordinates. Return nullptr if invalid value.
		Cell *getCell(std::size_t x, std::size_t y);

		// return cell ptr based on index in the array. Return nullptr if invalid value.
		Cell *getCell(std::size_t index);

		// configurate Map before generation
		void config(std::size_t width, std::size_t height, std::size_t start, std::size_t end);

		// start setter
		void setStart(std::size_t y);

		// end setter
		void setEnd(std::size_t y);

		// generate, return false if errors
		bool generate(PathGenerationOption option);

		// fill the map with a value
		void fill(const CellType &type);

		// return true if map correctly generated
		bool verify();

		// reset map
		void reset();

		// get map array
		inline const std::vector<Cell> &getArray() const
		{
			return _array;
		}

		inline std::size_t getWidth() const
		{
			return _width;
		}

		inline std::size_t getHeight() const
		{
			return _height;
		}

		bool saveToJson(const std::string &fileName) const; // Used to debug
		bool saveToBinary(const std::string &fileName) const; // Used at release time
		bool loadFromJson(const std::string &filename);
		bool loadFromBinary(const std::string &filename);

		bool printToFile(const std::string &filename) const; // Used to debug

		// serialization function
		template < typename Archive >
		void serialize(Archive &ar)
		{
			ar(cereal::make_nvp("Width", _width)
				, cereal::make_nvp("Height", _height)
				, cereal::make_nvp("Start", _start)
				, cereal::make_nvp("End", _end)
				, cereal::make_nvp("Array", _array)
				);
		}
	};
}