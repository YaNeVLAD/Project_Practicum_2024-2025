#include <cstdlib>
#include "src/Game.h"

int main()
{
	Game game;
	
	game.init();

	game.Run();

	return EXIT_SUCCESS;
}