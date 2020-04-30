#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"

using namespace Game;
using namespace Engine::Core::InputSystem;

PlayerController::PlayerController(const Input& input):_input{input}{}
PlayerController::~PlayerController(){}

void PlayerController::update()
{
    move();
}

void PlayerController::draw()
{
}

void move()
{

}