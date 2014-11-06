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
			_map.setInEditionMode(true);
			if (!_map.loadFromBinary("../" + path + ".bin"))
				_map.generate(PathGenerationOption::None);
			_map.setParent(this);

			subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
			{
				auto *m = static_cast<const Msg::Resize *>(msg);
				this->setRootArea((float)m->size.x, (float)m->size.y);
			});
		}

		virtual ~EditMapScene()
		{}

		virtual void update(const sf::Time &dt, sf::RenderWindow *window)
		{
			_map.update(dt, window);
		}
	protected:
		virtual bool _event(const sf::Event &event)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					_map.generatePath();
					if (_map.verify())
					{
						_map.saveToBinary("../" + _path + ".bin");
						_game->setMainMenu();
						return false;
					}
				}
			}
			return true;
		}
	private:
		Map _map;
		std::string _path;
	};
}