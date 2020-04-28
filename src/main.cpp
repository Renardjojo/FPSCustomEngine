#include "game.hpp"
#include "GE.hpp"

using namespace Engine;

int main()
{
    GE          gameEngine;

    Game::Game  game (gameEngine);

    game.run();

	return(EXIT_SUCCESS);
}