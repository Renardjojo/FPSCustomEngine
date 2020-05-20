#include "Game/game.hpp"
#include "Game/demo.hpp"
#include "GE/Core/System/TimeSystem.hpp"

using namespace Engine::Ressources;
using namespace Engine;
using namespace Engine::Core::System;

Game::Game::Game (Engine::GE& gameEngine)
    : gameEngine_ {gameEngine}
{}

void Game::Game::run ()
{
    Demo demo(gameEngine_);

    while(gameEngine_.gameState != E_GAME_STATE::EXIT)
    {   
        TimeSystem::update([&]()
                            { 
                                demo.fixedUpdate();
                            }, 
                            [&]() 
                            {
                                gameEngine_.pollEvent();
                                demo.update();
                            },
                            [&]()
                            {      
                                gameEngine_.clearRenderer();
                                demo.display();
                                gameEngine_.renderPresent();
                            });
    }
}