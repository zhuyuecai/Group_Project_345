#pragma once

#include "WindowManager.hpp"
#include "Map.hpp"
#include "Message.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "MapMessages.hpp"
#include "Critter.hpp"
#include "Modes/LaunchModeBehaviour.hpp"
#include "Modes/ChooseGameModeBehaviour.hpp"
#include "Modes/DemoMapBehaviour.hpp"
#include <memory>

namespace TDC
{
	class Game : public WindowManager, public PubSub
	{
	public:
		Game();
		virtual ~Game();
		void setPlayMode(const std::string &mapFilePath);
		void setChoosePlayMode(const std::string &mapFilePath);
		void setEditionMode(const std::string &mapFilePath);
		void setLaunchMode();	
		virtual void init();
	protected:
		virtual void _updateEvents(const sf::Event &event);
		virtual void _update(const sf::Time dt);
	private:
		std::unique_ptr < IModeBehaviour > _mode;
	};
}