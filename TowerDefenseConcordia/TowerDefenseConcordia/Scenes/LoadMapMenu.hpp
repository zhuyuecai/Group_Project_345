#pragma once 

#include "../WindowManager.hpp"
#include "../Map.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MapMessages.hpp"
#include "IModeBehaviour.hpp"
#include <memory>
#include <filesystem>
#include "../Game.hpp"

namespace TDC
{
	class LoadMapMenu : public Scene
	{
	public:
		LoadMapMenu()
		{
			subcribeToMessage<Msg::Resize>([this](const IMessage *msg)
			{
				auto *m = static_cast<const Msg::Resize *>(msg);
				this->setRootArea(m->size.x, m->size.y);
			});

			namespace fs = std::tr2::sys;
			fs::path path("../");
			fs::directory_iterator iter;
			if (fs::exists(path) && fs::is_directory(path))
			{
				for (fs::directory_iterator dir_iter(path); dir_iter != iter; ++dir_iter)
				{
					if (fs::is_regular_file(dir_iter->status()))
					{
						std::string name = dir_iter->path().filename();
						std::string end = ".bin";
						if (name.length() >= end.length() && 0 == name.compare(name.length() - end.length(), end.length(), end))
						{
							_files.push_back(name);
						}
					}
				}
			}

			assert(_font.loadFromFile("../assets/arial.ttf"));
		}

		virtual ~LoadMapMenu()
		{}

		virtual void update(const sf::Time &dt, sf::RenderWindow *window)
		{
			int c = 0;
			sf::Text text;
			text.setFont(_font);
			text.setCharacterSize(20);
			for (auto &e : _files)
			{
				if (c == 0)
				{
					text.setColor(sf::Color::Green);
				}
				else
				{
					text.setColor(sf::Color::Red);
				}
				++c;
				text.setString(e);
				text.setPosition(_pixels.left + _pixels.width / 2 - text.getLocalBounds().width / 2, _pixels.top + 40 * c);
				window->draw(text);
			}
			text.setColor(sf::Color::Cyan);
			text.setPosition(0, _pixels.top + _pixels.height - 100);
			text.setString("Space to scroll\nEnter To select\nEscape to exit");
			text.setCharacterSize(12);
			window->draw(text);
		}
	protected:
		virtual bool _event(const sf::Event &event)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					_game->setMainMenu();
						return false;
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					auto copy = _files.front();
					_files.pop_front();
					_files.push_back(copy);
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					if (_files.size() == 0)
						return true;
					_game->setPlayMode("../" + _files.front());
					return false;
				}

			}
			return true;
		}
	private:
		std::list < std::string > _files;
		sf::Font _font;
	};
}