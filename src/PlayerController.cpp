#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;

PlayerController::PlayerController(GameObject &gameObject,const Input& input):ScriptComponent(gameObject),_input{input}{}

void PlayerController::update()
{
    std::cout<<"AAHHHHHH"<<std::endl;
    move();
}

void PlayerController::move()
{

}