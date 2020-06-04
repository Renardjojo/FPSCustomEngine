#include "Game/PlayerController.hpp"
#include "GE/Core/Debug/assert.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/LowRenderer/ParticleSystemFactory.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/LootMachine.hpp"
#include "Game/UpgradeStation.hpp"
#include "Game/LifeDuration.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/ui.hpp"
#include "Game/LifeDuration.hpp"
#include "Game/LootMachine.hpp"
#include "Game/ParticuleGenerator.hpp"
#include <iostream>

#include <algorithm>
#include <math.h>

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
    _weaponIndex = 0;
    _mouseSpeed = 0.5f;
    _playerForce = 5.f;
    _playerMaxSpeed = 30.f;
    _jumpForce = 5.f;
    _airForce = 5.f;
    _groundForce = 150.f;
    _cameraYoffset = 5.f;
    _hitmarkerDisplayTime = 0.2f;
    _hitmarkerDisplaydelay = 0.f;
    _money = 500;
    _jump = false;
    _isGrounded = false;
    _flashLightOn = false;
    _maxLife = 500;
    _life = 500;

    _movement = Vec3{0.f, 0.f, 0.f};
    _direction = Vec3{0.f, 0.f, 0.f};

    _physics = _gameObject.getComponent<PhysicalObject>();
    GE_assertInfo(_physics != nullptr, "Game object must contain component \"PhysicalObject\"");
    _playerForce = _airForce;

    _flashLight = _gameObject.getChild("FlashLight")->getComponent<SpotLight>();
    GE_assertInfo(_flashLight != nullptr, "Game object name \"flashLight\" must contain component \"SpotLight\"");

    for (Image *image : UISystem::getImages())
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
  //   std::cout << "player is dead" << std::endl;

  if (_firearms.empty())
    return;

  if (_firearms.at(_weaponIndex)->isAutomatic() ? Input::mouse.leftClicDown
                                                : Input::mouse.leftClicDownOnce)
    shoot();

  // if (_life <= 0)
  //     std::cout << "player is dead" << std::endl;

  if (Input::mouse.rightClicDownOnce)
    switchAimState();

  if (Input::keyboard.getKeyState(Input::keyboard.use) == E_KEY_STATE::TOUCHED)
    activateLootMachine();

  if (Input::keyboard.getKeyState(Input::keyboard.reload) == E_KEY_STATE::TOUCHED)
  {
    _firearms.at(_weaponIndex)->reload();
  }

  if (_firearms.at(_weaponIndex)->getReloaded())
  {
    _firearms.at(_weaponIndex)->setReloaded(false);
    if (_bullet)
    {
      _bullet->value = bulletToString();
      _bullet->updateTexture();
    }
  }

  if (Input::mouse.wheel_scrollingFlag != 0)
  {
    if (_firearms.at(_weaponIndex)->isAiming())
    {
      _firearms.at(_weaponIndex)->switchAimState();
    }

    int mod = (Input::mouse.wheel_scrollingFlag + (int)_weaponIndex) % (int)_firearms.size();

    _firearms.at(_weaponIndex)->getGameObject().setActive(false);
    _weaponIndex = mod >= 0 ? mod : _firearms.size() + mod;
    _firearms.at(_weaponIndex)->getGameObject().setActive(true);

    if (_bullet)
    {
      _bullet->value = bulletToString();
      _bullet->updateTexture();
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
  _physics->setVelocity(_physics->getVelocity().clampLength(_playerMaxSpeed));
};

void PlayerController::switchFlashLightState()
{
  _flashLightOn = !_flashLightOn;
  _flashLight->enable(_flashLightOn);
}

void PlayerController::shoot()
{
  if (_firearms.at(_weaponIndex)->shoot(_gameObject.getGlobalPosition(), _gameObject.getModelMatrix().getVectorForward()))
    _hitmarker->isActive = true;

  if (_bullet)
  {
    _bullet->value = bulletToString();
    _bullet->updateTexture();
  }
}
void PlayerController::setCameraType(CameraType type) { _type = type; }

void PlayerController::switchAimState()
{
  _firearms.at(_weaponIndex)->switchAimState();
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

void PlayerController::camera()
{
  Vec2 mouseMotion{static_cast<float>(Input::mouse.motion.x), static_cast<float>(Input::mouse.motion.y)};
  mouseMotion *= _mouseSpeed * 0.02f;

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

  // Camera orbit
  // todo change distance
  Vec3 coordinates = cylindricalCoord(10.f, _orbit.y) + _gameObject.getPosition();
  coordinates.y += _cameraYoffset;
  _camera->setTranslation(coordinates);
  _camera->update();

  // lookat
  _camera->lookAt(_camera->getPosition(), _gameObject.getPosition(), Vec3::up);
}

void PlayerController::move()
{
  camera();

  // movements
  _direction.x = sinf(_orbit.y);
  _direction.y = 0;
  _direction.z = -cosf(_orbit.y);

  _movement = Vec3::zero;

  // movement
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

void PlayerController::onCollisionEnter(HitInfo &hitInfo)
{
  if (hitInfo.gameObject->getTag() == "Ground")
  {
    _isGrounded = true;
    _playerForce = _groundForce;
    // _physics->setIsKinematic(false);
  }
}

std::string PlayerController::bulletToString()
{
  return std::to_string(_firearms.at(_weaponIndex)->getMunition()) + " / " + std::to_string(_firearms.at(_weaponIndex)->getMagazine());
}

void PlayerController::addFirearm(Firearm *Firearm)
{
  GE_assert(Firearm != nullptr);

  _firearms.push_back(Firearm);

  if (_firearms.size() != 1)
  {
    _firearms.back()->getGameObject().setActive(false);
  }
}

Firearm *PlayerController::getCurrentFirearm()
{
  return _firearms.empty() ? nullptr : _firearms.at(_weaponIndex);
}

void PlayerController::inflictDamage(int damage) { _life -= damage; }

int *PlayerController::getLife() { return &_life; }

int *PlayerController::getMaxLife() { return &_maxLife; }

int *PlayerController::getMoney() { return &_money; }

void PlayerController::addMoney(int money) { _money += money; }

void PlayerController::setMoney(int money) { _money = money; }

void PlayerController::setBulletReference(Engine::Ressources::Title *bullet) { _bullet = bullet; }

void PlayerController::save(xml_document<> &doc, xml_node<> *nodeParent)
{
  if (!nodeParent)
    return;
  xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

  newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

  nodeParent->append_node(newNode);
}