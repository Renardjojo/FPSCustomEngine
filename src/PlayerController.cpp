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
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;
using namespace Engine::Core::InputSystem;

PlayerController::PlayerController(GameObject &gameObject) : ScriptComponent{gameObject},
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
    sincosf(angle - M_PI / 2, &res.z, &res.x);
    res *= 10;
    return res;
}

void PlayerController::move()
{
    //orbit
    _orbity += (Input::mouse.motion.x * M_PI / 180.f * 15.f * TimeSystem::getDeltaTime());
    Vec3 coordinates = coord((gameObject.entity.get()->getPosition() - _camera->getPosition()).length(), _orbity) + gameObject.entity.get()->getPosition();
    coordinates.y += _cameraYoffset;
    _camera->setTranslation(coordinates);
    _camera->update();

    //lookat
    _camera->lookAt(_camera->getPosition(), gameObject.entity.get()->getPosition(), Vec3{0.f, 1.f, 0.f});

    //movements
    _direction.x = sinf(_orbity);
    _direction.y = 0;
    _direction.z = -cosf(_orbity);

    //movement
    if (Input::keyboard.isDown[Input::keyboard.up])
        _movement -= _direction;

    if (Input::keyboard.isDown[Input::keyboard.down])
        _movement += _direction;

    if (Input::keyboard.isDown[Input::keyboard.left])
    {
        _movement.x -= _direction.z;
        _movement.z += _direction.x;
    }
    if (Input::keyboard.isDown[Input::keyboard.right])
    {
        _movement.x += _direction.z;
        _movement.z -= _direction.x;
    }

    if (Input::keyboard.onePressed(Input::keyboard.jump) == 1)
    {
        gameObject.getComponent<PhysicalObject>()->AddForce(0.f, 10.f, 0.f);
    }

    gameObject.entity.get()->setRotation({0.f, -_orbity, 0.f});
    //gameObject.entity.get()->translate(_movement * _playerSpeed * TimeSystem::getDeltaTime());
    gameObject.getComponent<PhysicalObject>()->AddForce(_movement * _playerSpeed * TimeSystem::getDeltaTime());

    _movement = {0.f, 0.f, 0.f};
}
