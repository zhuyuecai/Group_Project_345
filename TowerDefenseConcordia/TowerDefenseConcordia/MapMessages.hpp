#pragma once

#include "Message.hpp"
#include "Cell.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

namespace TDC
{
	namespace Msg
	{
		struct Resize : Message<Resize>
		{
			Resize(const sf::Vector2u &_size)
				: size(_size)
			{}

			sf::Vector2u size;
		};

		struct DrawMapArray : Message<DrawMapArray>
		{
			DrawMapArray(const std::vector<Cell> *_array)
				: array(_array)
			{}

			const std::vector<Cell> *array;
		};

		//message
		struct Event : Message < Event >
		{
			sf::Event event;
			Event(const sf::Event &e)
				: event(e)
			{}
		};

		struct PlayMode : Message < PlayMode >
		{
			enum Mode
			{
				MainMenu = 0,
				LoadMap,
				CreateMap,
				EditMap,
				Play,
				ChoosePlayMode
			};
			Mode mode;
			std::string argument;

			PlayMode(Mode _mode, std::string _argument = "")
				: mode(_mode)
				, argument(_argument)
			{}

			PlayMode() = delete;

		};
	}
}