#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include <math.h>
#include <algorithm>

using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;
using namespace Engine::Core::InputSystem;

PlayerController::PlayerController(GameObject &_gameObject) : ScriptComponent{_gameObject},
                                                             _camera{Camera::getCamUse()} {}

PlayerController::~PlayerController() {}

void PlayerController::start()
{
    _physics = _gameObject.getComponent<PhysicalObject>();
};

void PlayerController::update()
{
    move();
}

void PlayerController::fixedUpdate()
{
    if (_jump)
    {
        _physics->AddForce(0.f, 1.f, 0.f);
        _jump = false;
    }

    _physics->AddForce(_movement * _playerForce * TimeSystem::getDeltaTime());
};

void PlayerController::setCameraType(CameraType type)
{
    // if(_type!=type)
    // {
    //     //animation de transition de position de la camera
    // }
    _type = type;
}

void PlayerController::toggleCameraType()
{
    _type = _type == CameraType::FirstPerson ? CameraType::ThirdPerson : CameraType::FirstPerson;
}

Vec3 PlayerController::cylindricalCoord(float r, float angle)
{
    Vec3 res{Vec3::zero};
    sincosf(angle - M_PI_2f32, &res.z, &res.x);
    res *= r;
    return res;
}

void PlayerController::camera()
{
    Vec2 mouseMotion{static_cast<float>(Input::mouse.motion.x), static_cast<float>(Input::mouse.motion.y)};

    mouseMotion *= TimeSystem::getDeltaTime() * _mouseSpeed;

    _orbit.y += mouseMotion.x;
    _orbit.x += mouseMotion.y;

    if (_type == CameraType::FirstPerson)
    {
        _orbit.y = fmod(_orbit.y, M_PI * 2);
        _orbit.x = std::clamp(_orbit.x, -M_PI_2f32, M_PI_2f32);
        _gameObject.setRotation({0.f, -_orbit.y, 0.f});

        _camera->setTranslation(_gameObject.getPosition());
        _camera->setRotation({-_orbit.x, -_orbit.y + M_PIf32, 0.f});
        _camera->update();

        return;
    }

    //Camera orbit
    Vec3 coordinates = cylindricalCoord(10.f, _orbit.y) + _gameObject.getPosition();
    coordinates.y += _cameraYoffset;
    _camera->setTranslation(coordinates);
    _camera->update();

    //lookat
    _camera->lookAt(_camera->getPosition(), _gameObject.getPosition(), Vec3::up);
}

void PlayerController::move()
{
    _jump = Input::keyboard.isDown[Input::keyboard.jump];

    if (Input::keyboard.onePressed(SDL_SCANCODE_F2) == 1)
        toggleCameraType();

    camera();

    //movements
    _direction.x = sinf(_orbit.y);
    _direction.y = 0;
    _direction.z = -cosf(_orbit.y);

    _movement = Vec3::zero;

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
}
