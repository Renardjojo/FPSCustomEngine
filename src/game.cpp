#include "Game/game.hpp"
#include "Game/demo.hpp"

using namespace Engine::Ressources;
using namespace Engine;

Game::Game::Game (Engine::GE& gameEngine)
    : gameEngine_ {gameEngine}
{}

void Game::Game::run ()
{
    Demo demo(gameEngine_);

    while(gameEngine_.gameState != E_GAME_STATE::EXIT)
    {
        gameEngine_.pollEvent();
        gameEngine_.updateTime();

        demo.update();

        gameEngine_.clearRenderer();
        demo.display();
        gameEngine_.renderPresent();
    }
}