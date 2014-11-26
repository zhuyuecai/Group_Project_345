#pragma once

#include "WindowManager.hpp"
#include "Map.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "MapMessages.hpp"
#include "Critter.hpp"
#include "Scenes/LaunchModeBehaviour.hpp"
#include "Scenes/ChooseGameModeScene.hpp"
#include "Scenes/InGameScene.hpp"
#include <memory>

namespace TDC
{
	class Game : public WindowManager, public PubSub
	{
	public:
		Game();
		virtual ~Game();

		// Game
		void setPlayMode(const std::string &mapFilePath);
		// Choose game mode menu
		void setChoosePlayMode();
		// Edition menu
		void setEditionMenuMode();
		// Map creation mode
		void setMapCreationMode(const std::string &mapFilePath, std::size_t width, std::size_t height);
		// Main menu mode
		void setMainMenu();
		// Game Over mode
		void setGameOver(int s);
		// Load menu
		void setLoadMenu();
		// Initialization
		virtual void init();
	protected:
		virtual void _updateEvents(const sf::Event &event);
		virtual void _update(const sf::Time dt);
	private:
		std::unique_ptr < Scene > _mode;
	};
}