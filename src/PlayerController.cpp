#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include <math.h>

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::Time;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;

PlayerController::PlayerController(GameObject &gameObject, const Input &input) : ScriptComponent{gameObject},
                                                                                 _input{input},
                                                                                 _camera{Camera::getCamUse()}
{
}
PlayerController::~PlayerController() {}

void PlayerController::update()
{
    move();
}
Vec3 coord(float r, float angle)
{
    Vec3 res{0.f, 0.f, 0.f};
    sincosf(angle - M_PI/2, &res.z, &res.x);
    res *= 10;
    return res;
}

void PlayerController::move()
{    
    //orbit
    _orbity += (_input.mouse.motion.x * M_PI / 180);
    _camera->setTranslation(coord((gameObject.entity.get()->getPosition() - _camera->getPosition()).length(), _orbity) + gameObject.entity.get()->getPosition());
    _camera->update();
    
    //lookat
    _camera->lookAt(_camera->getPosition(), gameObject.entity.get()->getPosition(), Vec3{0.f, 1.f, 0.f});

    //movements
    _direction.x=sinf(_orbity);
    _direction.y = 0;
    _direction.z=-cosf(_orbity);

    //movement
    if (_input.keyboard.isDown[SDL_SCANCODE_UP])
        _movement -= _direction;

    if (_input.keyboard.isDown[SDL_SCANCODE_DOWN])
        _movement += _direction;
    
    if (_input.keyboard.isDown[SDL_SCANCODE_LEFT])
    {
        _movement.x -= _direction.z;
        _movement.z += _direction.x;
    }
    if (_input.keyboard.isDown[SDL_SCANCODE_RIGHT])
    {
        _movement.x += _direction.z;
        _movement.z -= _direction.x;
    }

    gameObject.entity.get()->translate(_movement * _playerSpeed * TimeSystem::getDeltaTime());


    _movement = {0.f, 0.f, 0.f};
}
