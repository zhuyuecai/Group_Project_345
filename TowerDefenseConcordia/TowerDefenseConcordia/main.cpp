
#include "Game.hpp"

#include <cassert>

int main(void)
{
	TDC::Game game;
	game.init();
	game.setLaunchMode();
//	game.generate();
	game.launch();
}