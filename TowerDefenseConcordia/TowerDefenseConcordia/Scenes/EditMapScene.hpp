#pragma once 

#include "../WindowManager.hpp"
#include "../Map.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MapMessages.hpp"
#include "IModeBehaviour.hpp"
#include <memory>

namespace TDC
{
	class EditMapScene : public Scene
	{
	public:
		EditMapScene(const std::string &path, int w, int h)
			: _path(path)
		{
			_map.config(w, h, 0, 0);
//			_map.setInEditionMode(true);
			_map.generate(PathGenerationOption::None);
			_map.setParent(this);


		}

		virtual ~EditMapScene()
		{}

		virtual void update(const sf::Time &dt, sf::RenderWindow *window)
		{
			_map.update(dt, window);
		}
	private:
		Map _map;
		std::string _path;
	};
}