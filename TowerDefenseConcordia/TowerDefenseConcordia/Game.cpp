#include "Game.hpp"
#include "Scenes/MapCreationMenu.hpp"
#include "Scenes/EditMapScene.hpp"
#include "Scenes/LoadMapMenu.hpp"
#include "Scenes/GameOverScene.hpp"
#include "Singleton.hh"

namespace TDC
{

#define RESET_SUBSCRIBER(S) \
	{ \
		removeSubscriber(S->getHandle()); \
        S->removeSubscriber(getHandle()); \
	    S.release(); \
	}

		Game::Game()
		{}

		Game::~Game()
		{}

		void Game::setPlayMode(const std::string &mapFilePath)
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<InGameScene>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			if (mapFilePath == "__RANDOM__")
			{
				static_cast<InGameScene*>(_mode.get())->generate();
			}
			else
			{
				static_cast<InGameScene*>(_mode.get())->load(mapFilePath);
			}
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setChoosePlayMode()
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<ChooseGameModeScene>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setEditionMenuMode()
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<MapCreationMenuBehaviour>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setMapCreationMode(const std::string &mapFilePath, std::size_t width, std::size_t height)
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<EditMapScene>(mapFilePath, width, height);
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setMainMenu()
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<LaunchModeBehaviour>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setGameOver(int s)
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<GameOver>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			((GameOver*)(_mode.get()))->setScore(s);
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setLoadMenu()
		{
			if (_mode)
				RESET_SUBSCRIBER(_mode);
			_mode = std::make_unique<LoadMapMenu>();
			_mode->setGamePtr(this);
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::init()
		{
			WindowManager::init();
			auto font = Singleton<sf::Font>::getInstance();
			assert(font->loadFromFile("../assets/arial.ttf"));
		}

		void Game::_updateEvents(const sf::Event &event)
		{
			publish<Msg::Event>(event);

			if (event.type == sf::Event::Resized)
			{
				publish<Msg::Resize>(_window.getSize());
				_window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)_window.getSize().x, (float)_window.getSize().y)));
			}
		}

		void Game::_update(const sf::Time dt)
		{
			if (_mode)
			{
				_mode->update(dt, &_window);
			}
		}
}