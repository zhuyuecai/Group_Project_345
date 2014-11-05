#pragma once

#include "Cell.hpp"
#include <vector>
#include <cereal/types/vector.hpp>

#include "ObserverPattern.hpp"
#include "RectArea.hpp"

namespace TDC
{

	enum PathGenerationOption : bool
	{
		Straight = 0
		, None = 1
	};

	enum Dir : unsigned char
	{
		NONE = 0
		, TOP
		, BOTTOM
		, LEFT
		, RIGHT
	};

	class Map : public RectArea
	{
	private:
		std::vector<Cell> _array;
		std::size_t _width;
		std::size_t _height;
		std::size_t _start;
		std::size_t _end;
		std::size_t _cellRatio;
		bool _editionMode;

		// return true if map configuations are correct
		inline bool _valid() const { return (_width != INVALID && _height != INVALID && _start < _height && _end < _height); }
	public:
		Map(); // ctor
		~Map(); // dtor

		inline void setInEditionMode(bool tof) { _editionMode = tof; }

		// Copy operators are canceled
		Map(const Map &o) = delete;
		Map(Map &&o) = delete;
		Map &operator=(const Map &o) = delete;
		Map &operator=(Map &&o) = delete;

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

		void generatePath();

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

		inline const std::vector<std::size_t>getNeighbours(std::size_t cell, CellType type)
		{
			std::vector<std::size_t> n;
			auto x = cell % _width;
			auto y = cell / _width;
			for (auto i = -1; i < 2; ++i) // x
			{
				for (auto j = -1; j < 2; ++j) // y
				{
					auto nx = x + i;
					auto ny = y + j;

					if (i == 0 && j == 0)
						continue;
					if (i == 0 || j == 0)
					{
						auto c = getCell(nx, ny);
						if (c && c->_type == type)
						{
							n.push_back(c->_index);
						}
					}
				}
			}
			return n;
		}

		std::size_t getCellIndexFromPixels(int x, int y);
		sf::Vector2i getPixelPositionForCell(std::size_t i) const;

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
		virtual void update(const sf::Time &dt, sf::RenderWindow *window);

		inline std::size_t getCellRatio() const { return _cellRatio; }
	protected:
		virtual void _resized()
		{
			auto w = _pixels.width;
			auto h = _pixels.height;
			_cellRatio = w / _width;
			_cellRatio = h / _height < _cellRatio ? h / _height : _cellRatio;
		}

		virtual bool _event(const sf::Event &event);
	private:
	};
}