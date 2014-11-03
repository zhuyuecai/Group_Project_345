#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include "ObserverPattern.hpp"

namespace TDC
{
	// Messages used by Critters
	namespace CritterMsg
	{
		struct GetNextCell : Message < GetNextCell >
		{
			GetNextCell(std::size_t currentCell, std::size_t *nextCell)
				: _current(currentCell)
				, _next(nextCell)
			{}

			std::size_t _current;
			std::size_t *_next;
		};

		struct GetStart : Message < GetStart >
		{
			GetStart(std::size_t *start)
				: _start(start)
			{}

			std::size_t *_start;
		};
	}

	class Critter : public PubSub
	{
		public:

			enum CritterType
			{
				Type1 = 0,
				Type2 = 1
			};

			Critter(CritterType type)
				: _speed(0)
				, _cellStage(0)
				, _currentCell(0)
				, _destinationCell(0)
			{
				switch (type)
				{
				case TDC::Critter::Type1:
					_speed = 2.0f;
					_color = sf::Color::Green;
					break;
				case TDC::Critter::Type2:
					_speed = 3.0f;
					_color = sf::Color::Magenta;
					break;
				default:
					break;
				}
			}

			void init()
			{
				publish<CritterMsg::GetStart>(&_currentCell);
				publish<CritterMsg::GetNextCell>(_currentCell, &_destinationCell);
			}

			bool update(const sf::Time &dt)
			{
				auto advancement = _speed / (1000.0f / (float)dt.asMilliseconds());
				_cellStage += advancement;
				if (_cellStage >= 1.0f)
				{
					while (_cellStage >= 1.0f)
					{
						_currentCell = _destinationCell;
						publish<CritterMsg::GetNextCell>(_destinationCell, &_destinationCell);
						_cellStage -= 1.0f;
					}
				}
				if (_currentCell == std::size_t(-1) || _destinationCell == std::size_t(-1))
					return false;
				return true;
			}

			inline std::size_t getCurrentCell() const { return _currentCell; }
			inline std::size_t getDestinationCell() const { return _destinationCell; }
			inline float getCellStage() const { return _cellStage; }
			inline const sf::Color &getColor() const { return _color; }
	private:
		float _speed; //speed of the critter
		float _cellStage; //normalized value of the position of the critter on the cell [0.0f -> 1.0f]
		sf::Color _color;
		std::size_t _currentCell; // cell the critter is on
		std::size_t _destinationCell; //destination cell
	};
}