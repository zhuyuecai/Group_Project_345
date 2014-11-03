#include "Game.hpp"
#include "Modes/MapCreationMenu.hpp"

namespace TDC
{
		Game::Game()
		{}

		Game::~Game()
		{}

		void Game::setPlayMode(const std::string &mapFilePath)
		{
			if (_mode)
			{
				removeSubscriber(_mode->getHandle());
				_mode->removeSubscriber(getHandle());
				_mode.release();
			}
			_mode = std::make_unique<DemoMap>();
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			if (mapFilePath == "__RANDOM__")
			{
				static_cast<DemoMap*>(_mode.get())->generate();
			}
			else
			{
				static_cast<DemoMap*>(_mode.get())->load(mapFilePath);
			}
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setChoosePlayMode(const std::string &mapFilePath)
		{
			if (_mode)
			{
				removeSubscriber(_mode->getHandle());
				_mode->removeSubscriber(getHandle());
				_mode.release();
			}
			_mode = std::make_unique<ChooseGameModeBehaviour>();
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setEditionMode(const std::string &mapFilePath)
		{
			if (_mode)
			{
				removeSubscriber(_mode->getHandle());
				_mode->removeSubscriber(getHandle());
				_mode.release();
			}
			_mode = std::make_unique<MapCreationMenuBehaviour>();
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::setLaunchMode()
		{
			if (_mode)
			{
				removeSubscriber(_mode->getHandle());
				_mode->removeSubscriber(getHandle());
			}
			_mode = std::make_unique<LaunchModeBehaviour>();
			addSubscriber(_mode->getHandle());
			_mode->addSubscriber(getHandle());
			_mode->init();
			// we publish the size of the window to resize buttons
			publish<Msg::Resize>(_window.getSize());
		}

		void Game::init()
		{
			WindowManager::init();

			subcribeToMessage<Msg::PlayMode>([this](const IMessage *msg){
				const auto *m = static_cast<const Msg::PlayMode*>(msg);
				switch (m->mode)
				{
				case Msg::PlayMode::CreateMap:
					setEditionMode(m->argument);
					break;
				case Msg::PlayMode::EditMap:
					setEditionMode(m->argument);
					break;
				case Msg::PlayMode::LoadMap:
					setPlayMode(m->argument);
					break;
				case Msg::PlayMode::MainMenu:
					setLaunchMode();
					break;
				case Msg::PlayMode::Play:
					setPlayMode(m->argument);
					break;
				case Msg::PlayMode::ChoosePlayMode:
					setChoosePlayMode(m->argument);
					break;
				default:
					break;
				}
			});
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