#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/LowRenderer/ParticleSystemFactory.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/LootMachine.hpp"
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
      _rm{t_RessourcesManager::getRessourceManagerUse()},
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
    _physics = _gameObject.getComponent<PhysicalObject>();
    GE_assertInfo(_physics != nullptr, "Game object must contain component \"PhysicalObject\"");
    _playerForce = _airForce;

    _flashLight = _gameObject.getChild("FlashLight")->getComponent<SpotLight>();
    GE_assertInfo(_flashLight != nullptr, "Game object name \"flashLight\" must contain component \"SpotLight\"");
};

void PlayerController::update()
{

    if (Input::keyboard.getKeyState(Input::keyboard.jump) == E_KEY_STATE::TOUCHED)
        _jump = true;

    if (Input::keyboard.getKeyState(Input::keyboard.switchFlashLightState) == E_KEY_STATE::TOUCHED)
        switchFlashLightState();

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F2) == E_KEY_STATE::TOUCHED)
        toggleCameraType();

    move();

    if (_life <= 0)
        std::cout << "player is dead" << std::endl;
    if (!_firesGuns.empty() && (_firesGuns[0]->isAutomatic() ? Input::mouse.leftClicDown : Input::mouse.leftClicDownOnce))
    {
        shoot();
    }

    if (Input::keyboard.getKeyState(SDL_SCANCODE_R) == E_KEY_STATE::TOUCHED)
    {
        activateLootMachine();
    }

    /*Choose fire gun*/
    if (_firesGuns.size() > 1)
    {
        if (Input::mouse.wheel_scrollingFlag != 0)
        {
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
    _firesGuns[0]->shoot(_gameObject.getGlobalPosition(), _gameObject.getModelMatrix().getVectorForward());

    //     HitInfo rayInfo;
    //     Vec3 shootDirection = _gameObject.getModelMatrix().getVectorForward();
    //     if (PhysicSystem::triggerRayCast("Bullet", _gameObject.getGlobalPosition() + shootDirection * 6.f, shootDirection, 10000.f, rayInfo))
    //     {
    //         GameObjectCreateArg decaleGOPref {"bulletHoleDecal", rayInfo.intersectionsInfo.intersection1};
    //         decaleGOPref.transformArg.scale = Vec3::one / 20.f;
    //         ModelCreateArg      modelDecaleGOPref   {&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("LightAndTexture"),
    //                                                 &t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>("BulletHole"),
    //                                                 &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("Plane"),
    //                                                 "LightAndTexture",
    //                                                 {"BulletHole"},
    //                                                 "Plane"};
    // /*
    //         ModelCreateArg modelArg3{&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("Color"),
    //                                 &t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>("RedMaterial"),
    //                                 &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("PlaneZ"),
    //                                 "Color",
    //                                 {"RedMaterial"},
    //                                 "PlaneZ"};

    //         ParticuleGenerator::ParticleSystemCreateArg particalArg;
    //         particalArg.modelCreateArg = modelArg3;
    //         particalArg.isBillBoard = true;
    //         particalArg.physicalObjectCreateArg.useGravity = true;
    //         particalArg.useScaledTime = true;
    //         particalArg.velocityEvolutionCoef = 1.f;
    //         particalArg.spawnCountBySec = 100.f;
    //         particalArg.lifeDuration = 0.5f;
    //         particalArg.physicalObjectCreateArg.mass = 1.f;
    //         particalArg.scale = {0.05, 0.05, 0.05};

    //         GameObject& particleGO = Scene::getCurrentScene()->add<GameObject>(Scene::getCurrentScene()->getWorld(), GameObjectCreateArg{"ParticleContenerBlood", {rayInfo.intersectionsInfo.intersection1}});
    //         particleGO.addComponent<ParticuleGenerator>(particalArg);
    //         particleGO.addComponent<LifeDuration>(3.f);*/

    //         ParticleSystemFactory::createDecale(Scene::getCurrentScene()->getGameObject("world/DecalContenor"), decaleGOPref, modelDecaleGOPref, rayInfo.intersectionsInfo.normalI1);
    //         if (rayInfo.gameObject->getTag() != "Ground")
    //             rayInfo.gameObject->destroy();
    //     }
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
    Scene::getCurrentScene()->getGameObject("LootMachine").getComponent<LootMachine>()->activate(_gameObject.getGlobalPosition());
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
        // _physics->setIsKinematic(false);
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