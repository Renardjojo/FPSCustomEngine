#include "Game/game.hpp"
#include "GE/GE.hpp"

#include <functional>
 
using namespace Engine;

int main()
{
    GE          gameEngine;

    Game::Game  game (gameEngine);

    game.run();

	return(EXIT_SUCCESS);
}