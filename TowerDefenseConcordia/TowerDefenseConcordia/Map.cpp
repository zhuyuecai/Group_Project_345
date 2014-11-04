#include "Map.hpp"
#include <fstream>
#include "MapMessages.hpp"
#include "Critter.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace TDC
{
	Map::Map()
		: _width(INVALID)
		, _height(INVALID)
		, _start(0)
		, _end(0)
	{
		setCentered(false);
	}

	Map::~Map()
	{}

	Cell* Map::getCell(std::size_t x, std::size_t y)
	{
		if (x >= _width || y >= _height || !_valid())
			return nullptr;
		return &_array[x + y * _width];
	}

	Cell *Map::getCell(std::size_t index)
	{
		if (index >= _array.size() || !_valid())
			return nullptr;
		return &_array[index];
	}

	void Map::config(std::size_t width, std::size_t height, std::size_t start, std::size_t end)
	{
		if (width == 0 || height == 0)
		{
			std::cerr << "Error: Invalid map size" << std::endl;
			return;
		}
		_width = width;
		_height = height;
		_start = start;
		_end = end;
		//publish<Msg::Resize>(sf::Vector2u(width, height));

		subcribeToMessage<CritterMsg::GetNextCell>([this](IMessage *msg)
		{
			auto m = static_cast<CritterMsg::GetNextCell*>(msg);
			auto c = getCell(m->_current);
			if (!c)
			{
				*m->_next = -1;
				return;
			}
			*m->_next = c->getNext();
		});

		subcribeToMessage<CritterMsg::GetStart>([this](IMessage *msg)
		{
			auto m = static_cast<CritterMsg::GetStart*>(msg);
			*m->_start = this->_start * _width;
		});
	}

	void Map::setStart(std::size_t y)
	{
		_array[_start * _width]._type = CellType::Wall;
		_start = y;
		_array[_start * _width]._type = CellType::Path;
	}

	void Map::setEnd(std::size_t y)
	{
		_array[_end * _width + _width]._type = CellType::Wall;
		_end = y;
		_array[_end * _width + _width]._type = CellType::Path;
	}

	bool Map::generate(PathGenerationOption option)
	{
		if (!_valid())
			return false;
		fill(CellType::Wall);
		if (option == PathGenerationOption::Straight)
		{
			std::size_t y = _start;
			std::size_t past = _start * _width;
			for (std::size_t x = 0; x < _width; ++x)
			{
				std::size_t index = x + y * _width;
				_array[index].setType(CellType::Path);
				if (index != past)
					_array[past].setNext(index);
				past = index;
				if (y != _end)
				{
					if (y < _end)
					{
						index = x + ++y * _width;
						_array[index].setType(CellType::Path);
						if (index != past)
							_array[past].setNext(index);
						past = index;
					}
					else
					{
						index = x + --y * _width;
						_array[index].setType(CellType::Path);
						if (index != past)
							_array[past].setNext(index);
						past = index;
					}
				}
			}
			while (y != _end)
			{
				auto x = _width - 1;
				auto index = x + y * _width;
				if (y < _end)
					{
						index = x + ++y * _width;
						_array[index].setType(CellType::Path);
						if (index != past)
							_array[past].setNext(index);
						past = index;
					}
					else
					{
						index = x + --y * _width;
						_array[index].setType(CellType::Path);
						if (index != past)
							_array[past].setNext(index);
						past = index;
					}
			}
		}
		generatePath();
		return true;
	}

	void Map::fill(const CellType &type)
	{
		if (!_valid())
			return;
		std::size_t index = 0;
		_array.resize(_width * _height);
		for (auto &e : _array)
		{
			e.setType(type);
			e.setNext(INVALID);
			e.setIndex(index++);
		}
	}

	bool Map::verify()
	{
		if (!_valid())
			return false;
		if (!getCell(0, _start)->hasNext())
			return false;
		auto cell = getCell(getCell(0, _start)->getNext());
		auto end = getCell(_width - 1, _end);
		while (cell != end)
		{
			if (!cell->hasNext())
				return false;
			cell = getCell(cell->getNext());
		}
		return true;
	}

	// Used to debug
	bool Map::saveToJson(const std::string &filename) const
	{
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open())
			return false;
		{
			cereal::JSONOutputArchive ar(file);
			ar(*this);
		}
		file.close();
		return true;
	}

		// Used at release time
	bool Map::saveToBinary(const std::string &filename) const
	{
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open())
			return false;
		{
			cereal::PortableBinaryOutputArchive ar(file);
			ar(*this);
		}
		file.close();
		return true;
	}

	bool Map::loadFromJson(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
			return false;
		{
			cereal::JSONInputArchive ar(file);
			ar(*this);
		}
		file.close();
		auto returnValue = verify();
		config(_width, _height, _start, _end);
		return returnValue;
	}

	bool Map::loadFromBinary(const std::string &filename)
	{
		std::ifstream file(filename, std::ios::binary);
		if (!file.is_open())
			return false;
		{
			cereal::PortableBinaryInputArchive ar(file);
			ar(*this);
		}
		file.close();
		auto returnValue = verify();
		config(_width, _height, _start, _end);
		return returnValue;
	}

	bool Map::printToFile(const std::string &filename) const
	{
		std::ofstream file(filename, std::ios::binary);
		if (!file.is_open())
			return false;
		{
			std::size_t x = 0;
			for (auto &e : _array)
			{
				file << (int)e.getType();
				++x;
				if (x >= _width)
				{
					file << std::endl;
					x = 0;
				}
			}
		}
		file.close();
		return true;
	}


	void Map::generatePath()
	{
		for (auto &e : _array)
			e._rank = INVALID;
		_rankCell(_width - 1, _end, 0);
		auto d = _start * _width;
		auto *c = getCell(d);
		while (true)
		{
			if (c->_rank == 0)
				return;
			auto nextX = getCell(d + 1);
			auto nextY = getCell(d + _width);
			auto prevY = getCell(d - _width);
			if (nextX && nextX->_rank == c->_rank - 1)
			{
				c->setNext(nextX->_index);
				c = nextX;
			}
			else if (nextY && nextY->_rank == c->_rank - 1)
			{
				c->setNext(nextY->_index);
				c = nextY;
			}
			else if (prevY && prevY->_rank == c->_rank - 1)
			{
				c->setNext(prevY->_index);
				c = prevY;
			}
			else
			{
				std::cerr << "error !!!";
			}
			d = c->_index;
		}
	}

	void Map::update(const sf::Time &dt, sf::RenderWindow *window)
	{
		sf::RectangleShape rectangle;
		rectangle.setFillColor(sf::Color(150, 50, 250));
		rectangle.setSize(sf::Vector2f((float)_cellRatio, (float)_cellRatio));

		for (std::size_t i = 0; i < _array.size(); ++i)
		{
			auto &e = _array[i];
			if (e.getType() == CellType::Wall)
			{
				rectangle.setPosition((float)((i % _width) * _cellRatio), (float)((i / _width) * _cellRatio));
				window->draw(rectangle);
			}
		}
	}
}