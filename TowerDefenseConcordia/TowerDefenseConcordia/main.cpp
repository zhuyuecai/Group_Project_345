
#include "Game.hpp"

#include <cassert>

int main(void)
{
	// Game creation
	TDC::Game game;

	// Game initialisation
	game.init();

	// We set Game mode (main menu)
	game.setMainMenu();

	// We launch game loop
	game.launch();
}