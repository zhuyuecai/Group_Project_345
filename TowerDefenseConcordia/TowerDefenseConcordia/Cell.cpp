#include "Cell.hpp"
#include <utility>

namespace TDC
{
	Cell::Cell()
		: _type(CellType::Wall)
		, _index(INVALID)
		, _next(INVALID)
	{

	}

	Cell::~Cell()
	{}

	Cell::Cell(const Cell &o)
		: _type(o._type)
		, _index(o._index)
		, _next(o._next)
	{

	}

	Cell::Cell(Cell &&o)
		: _type(std::move(o._type))
		, _index(std::move(o._index))
		, _next(std::move(o._next))
	{

	}

	Cell &Cell::operator=(const Cell &o)
	{
		_type = o._type;
		_index = o._index;
		_next = o._next;
		return *this;
	}

	Cell &Cell::operator=(Cell &&o)
	{
		_type = std::move(o._type);
		_index = std::move(o._index);
		_next = std::move(o._next);
		return *this;
	}

}