#include "Game/game.hpp"
#include "Game/demo.hpp"

using namespace Engine::Resources;

Game::Game::Game (Engine::GE& gameEngine)
    : gameEngine_ {gameEngine}
{}

void Game::Game::run ()
{
    Demo demo(gameEngine_);

    while(!gameEngine_.getKeyboardEvent().escIsRelease)
    {
        gameEngine_.pollEvent();
        gameEngine_.updateTime();

        demo.update();

        gameEngine_.clearRenderer();
        demo.display();
        gameEngine_.renderPresent();
    }
}