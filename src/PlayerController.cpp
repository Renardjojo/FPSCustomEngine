#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/LowRenderer/ParticleSystemFactory.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/LootMachine.hpp"
#include "Game/UpgradeStation.hpp"
#include "Game/LifeDuration.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Core/Debug/assert.hpp"

#include <math.h>
#include <algorithm>

using namespace Game;
using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::System;
using namespace Engine::Core::Maths;
using namespace Engine::LowRenderer;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::Maths::ShapeRelation;
using namespace Engine::LowRenderer;

PlayerController::PlayerController(GameObject &_gameObject)
    : ScriptComponent{_gameObject},
      _camera{Camera::getCamUse()}
{
    _name = __FUNCTION__;
}

Vec3 PlayerController::cylindricalCoord(float r, float angle)
{
    Vec3 res{Vec3::zero};
    sincosf(angle - M_PI_2f32, &res.z, &res.x);
    res *= r;
    return res;
}

void PlayerController::start()
{
    _orbit = Vec2{0.f,0.f};
    _mouseSpeed = 0.5f;
    _playerForce = 30.f;
    _jumpForce = 7.f;
    _airForce = 5.f;
    _groundForce = 30.f;
    _playerMaxSpeed = 1.f;
    _cameraSpeed = 10.f;
    _cameraYoffset = 5.f;
    _hitmarkerDisplayTime = 0.2f;
    _hitmarkerDisplaydelay = 0.f;
    _life = 5;
    _money = 500;
    _jump = false;
    _isGrounded = false;

    _movement = Vec3{0.f, 0.f, 0.f};
    _direction = Vec3{0.f, 0.f, 0.f};

    _physics = _gameObject.getComponent<PhysicalObject>();
    GE_assertInfo(_physics != nullptr, "Game object must contain component \"PhysicalObject\"");
    _playerForce = _airForce;

    _flashLight = _gameObject.getChild("FlashLight")->getComponent<SpotLight>();
    GE_assertInfo(_flashLight != nullptr, "Game object name \"flashLight\" must contain component \"SpotLight\"");

    for (Image* image : UISystem::getImages())
    {
        if (image->getName().compare("HitMarker") == 0)
        {
            _hitmarker = image;
            break;
        }
    }
    GE_assertInfo(_hitmarker != nullptr, "no hitmarker found");
};

void PlayerController::update()
{
    if (Input::keyboard.getKeyState(Input::keyboard.jump) == E_KEY_STATE::DOWN)
        _jump = true;

    if (Input::keyboard.getKeyState(Input::keyboard.flashLight) == E_KEY_STATE::TOUCHED)
        switchFlashLightState();

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F2) == E_KEY_STATE::TOUCHED)
        toggleCameraType();

    move();

    // if (_life <= 0)
    //     std::cout << "player is dead" << std::endl;

    if (!_firesGuns.empty() && (_firesGuns[0]->isAutomatic() ? Input::mouse.leftClicDown : Input::mouse.leftClicDownOnce))
        shoot();

    if (!_firesGuns.empty() && Input::mouse.rightClicDownOnce)
        switchAimState();

    if (Input::keyboard.getKeyState(Input::keyboard.use) == E_KEY_STATE::TOUCHED)
        activateLootMachine();

    if (Input::keyboard.getKeyState(Input::keyboard.reload) == E_KEY_STATE::TOUCHED)
        _firesGuns[0]->reload();

    /*Choose fire gun*/
    if (_firesGuns.size() > 1)
    {
        if (Input::mouse.wheel_scrollingFlag != 0)
        {
            if (_firesGuns.front()->isAiming())
            {
                _firesGuns.front()->switchAimState();
            }

            if (Input::mouse.wheel_scrollingFlag == 1)
            {
                Firearm *temp = _firesGuns.front();
                _firesGuns.erase(_firesGuns.begin());
                _firesGuns.push_back(temp);
                temp->getGameObject().setActive(false);
                _firesGuns.front()->getGameObject().setActive(true);
            }
            else
            {
                Firearm *temp = _firesGuns.back();
                _firesGuns.pop_back();
                temp->getGameObject().setActive(true);
                _firesGuns.front()->getGameObject().setActive(false);
                _firesGuns.insert(_firesGuns.begin(), temp);
            }
        }
    }
    if (_hitmarker->isActive)
    {
        _hitmarkerDisplaydelay += TimeSystem::getDeltaTime();
        if (_hitmarkerDisplaydelay > _hitmarkerDisplayTime)
        {
            _hitmarkerDisplaydelay = 0.f;
            _hitmarker->isActive = false;
        }
    }
}

void PlayerController::fixedUpdate()
{
    if (_jump && _isGrounded)
    {
        _physics->addForce(Vec3::up * _jumpForce);
        _jump = false;
        _playerForce = _airForce;
        _physics->setUseGravity(true);
        _isGrounded = false;
    }
    _jump = false;

    _physics->addForce(_movement * _playerForce * TimeSystem::getFixedDeltaTime());
};

void PlayerController::switchFlashLightState()
{
    _flashLightOn = !_flashLightOn;
    _flashLight->enable(_flashLightOn);
}

void PlayerController::shoot()
{
    if (_firesGuns[0]->shoot(_gameObject.getGlobalPosition(), _gameObject.getModelMatrix().getVectorForward()))
        _hitmarker->isActive = true;
}

void PlayerController::switchAimState()
{
    _firesGuns[0]->switchAimState();
}

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

void PlayerController::activateLootMachine()
{
    Scene::getCurrentScene()->getGameObject("LootMachine").getComponent<LootMachine>()->activate(_gameObject.getGlobalPosition(), _money);
    Scene::getCurrentScene()->getGameObject("MunitionCapacityUpgradeStation").getComponent<MunitionCapacityUpgradeStation>()->activate(_gameObject.getGlobalPosition(), this);
}

void PlayerController::addFirearm(Firearm *Firearm)
{
    GE_assert(Firearm != nullptr);

    _firesGuns.push_back(Firearm);

    if (_firesGuns.size() != 1)
    {
        _firesGuns.back()->getGameObject().setActive(false);
    }
}

void PlayerController::camera()
{
    Vec2 mouseMotion{static_cast<float>(Input::mouse.motion.x), static_cast<float>(Input::mouse.motion.y)};
    mouseMotion *= _mouseSpeed * TimeSystem::getDeltaTime();

    _orbit.y += mouseMotion.x;
    _orbit.x += mouseMotion.y;

    _orbit.y = fmod(_orbit.y, M_PI * 2);
    _orbit.x = std::clamp(_orbit.x, -M_PI_2f32, M_PI_2f32);

    if (_type == CameraType::FirstPerson)
    {
        _gameObject.setRotation({_orbit.x, -_orbit.y, 0.f});
        _camera->setTranslation(_gameObject.getPosition());
        _camera->setRotation({-_orbit.x, -_orbit.y + M_PIf32, 0.f});
        _camera->update();

        return;
    }

    //Camera orbit
    //todo change distance
    Vec3 coordinates = cylindricalCoord(10.f, _orbit.y) + _gameObject.getPosition();
    coordinates.y += _cameraYoffset;
    _camera->setTranslation(coordinates);
    _camera->update();

    //lookat
    _camera->lookAt(_camera->getPosition(), _gameObject.getPosition(), Vec3::up);
}

void PlayerController::inflictDamage(int damage)
{
    _life -= damage;
}

void PlayerController::move()
{
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

    //clamp max speed
}

void PlayerController::onCollisionEnter(HitInfo &hitInfo)
{
    if (hitInfo.gameObject->getTag() == "Ground")
    {
        _isGrounded = true;
        _playerForce = _groundForce;
    }
}

void PlayerController::save(xml_document<> &doc, xml_node<> *nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    nodeParent->append_node(newNode);
}