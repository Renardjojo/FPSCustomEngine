#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include <math.h>

using namespace Game;
using namespace Engine::Physics;
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
Vec3 coord(float r, float angle,const Vec3& pos)
{
    Vec3 res{0.f,0.f, 0.f};
    sincosf(angle - M_PI/2, &res.z, &res.x);
    res *= 10;
    res.y=pos.y;
    return res;
}

void PlayerController::move()
{    
    std::cout << _camera->getPosition() << std::endl;
    //orbit
    _camera->setTranslation({_camera->getPosition().x, 5.f, _camera->getPosition().z});
    _orbity += (_input.mouse.motion.x * M_PI / 180);
    _camera->setTranslation(coord((gameObject.entity.get()->getPosition() - _camera->getPosition()).length(), _orbity,_camera->getPosition()) + gameObject.entity.get()->getPosition());
    _camera->update();
    
    //lookat
    _camera->lookAt(_camera->getPosition(), gameObject.entity.get()->getPosition(), Vec3{0.f, 1.f, 0.f});

    //movements
    _direction.x=sinf(_orbity);
    _direction.y = 0;
    _direction.z=-cosf(_orbity);

    //movement
    if (_input.keyboard.isDown[_input.keyboard.up])
        _movement -= _direction;

    if (_input.keyboard.isDown[_input.keyboard.down])
        _movement += _direction;
    
    if (_input.keyboard.isDown[_input.keyboard.left])
    {
        _movement.x -= _direction.z;
        _movement.z += _direction.x;
    }
    if (_input.keyboard.isDown[_input.keyboard.right])
    {
        _movement.x += _direction.z;
        _movement.z -= _direction.x;
    }

    gameObject.entity.get()->translate(_movement * _playerSpeed * TimeSystem::getDeltaTime());

    _movement = {0.f, 0.f, 0.f};
}
