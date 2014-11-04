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
		void setChoosePlayMode(const std::string &mapFilePath);
		// Edition menu
		void setEditionMenuMode(const std::string &mapFilePath);
		// Map creation mode
		void setMapCreationMode(const std::string &mapFilePath);
		// Main menu mode
		void setLaunchMode();	
		// Initialization
		virtual void init();
	protected:
		virtual void _updateEvents(const sf::Event &event);
		virtual void _update(const sf::Time dt);
	private:
		std::unique_ptr < Scene > _mode;
	};
}