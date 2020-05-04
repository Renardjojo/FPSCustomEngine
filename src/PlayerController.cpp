#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::Time;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

PlayerController::PlayerController(GameObject &gameObject, const Input &input) : ScriptComponent{gameObject},
                                                                                 _input{input} {}
PlayerController::~PlayerController() {}

void PlayerController::update()
{
    move();
}

void PlayerController::move()
{
    if (_input.keyboard.isDown[SDL_SCANCODE_UP])
        _movement.z--;

    if (_input.keyboard.isDown[SDL_SCANCODE_DOWN])
        _movement.z++;

    if (_input.keyboard.isDown[SDL_SCANCODE_LEFT])
        _movement.x--;

    if (_input.keyboard.isDown[SDL_SCANCODE_RIGHT])
        _movement.x++;

    gameObject.entity.get()->translate(_movement.getNormalize() * _playerSpeed * TimeSystem::getDeltaTime());

    _mouseMovements = {_input.mouse.motion.y / 100.f, _input.mouse.motion.x / 100.f, 0.f};

    //Vec3 m = gameObject.entity.get()->getPosition() - Camera::getCamUse()->getPosition();
    //std::cout<<"Cam : "<<Camera::getCamUse()->getPosition()<<" Player : "<< gameObject.entity.get()->getPosition()<<std::endl;
    Camera::getCamUse()->lookAt(Camera::getCamUse()->getPosition(), gameObject.entity.get()->getPosition(), Vec3{0.f, 1.f, 0.f});

    _movement = {0.f, 0.f, 0.f};
    _mouseMovements = {0.f, 0.f, 0.f};
}