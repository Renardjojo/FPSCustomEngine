#include <iostream>
#include "Game/PlayerController.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/LowRenderer/ParticleSystemFactory.hpp"
#include "Game/ParticuleGenerator.hpp"
#include "Game/LifeDuration.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/ressourcesManager.hpp"

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
{}


void PlayerController::start()
{
    _physics = _gameObject.getComponent<PhysicalObject>();
};

void PlayerController::update()
{
    move();

    if (Input::mouse.leftClicDownOnce)
    {
        shoot();
    }
}

void PlayerController::fixedUpdate()
{
    if (_jump)
    {
        _physics->addForce(0.f, 1.f, 0.f);
        _jump = false;
    }
    
    _physics->addForce(_movement * _playerForce * TimeSystem::getDeltaTime());
};

void PlayerController::shoot()
{
    HitInfo rayInfo;
    Vec3 shootDirection = _gameObject.getModelMatrix().getVectorForward();
    if (PhysicSystem::rayCast(_gameObject.getGlobalPosition() + shootDirection * 2.f, shootDirection, 10000.f, rayInfo))
    {
        GameObjectCreateArg decaleGOPref {"bulletHoleDecal", rayInfo.intersectionsInfo.intersection1};
        ModelCreateArg      modelDecaleGOPref   {&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("TextureOnly"), 
                                                {&t_RessourcesManager::getRessourceManagerUse()->get<Material>("BulletHole")}, 
                                                &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("Plane"),
                                                "TextureOnly", 
                                                {"BulletHole"}, 
                                                "Plane"};

        ModelCreateArg modelArg3{&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("Color"),
                                {&t_RessourcesManager::getRessourceManagerUse()->get<Material>("RedMaterial")},
                                &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("Plane"),
                                "Color",
                                {"RedMaterial"},
                                "Plane"};

        ParticuleGenerator::ParticleSystemCreateArg particalArg;
        particalArg.modelCreateArg = modelArg3;
        particalArg.isBillBoard = true;
        particalArg.physicalObjectCreateArg.useGravity = true;
        particalArg.useScaledTime = true;
        particalArg.velocityEvolutionCoef = 1.f;
        particalArg.spawnCountBySec = 100.f;
        particalArg.lifeDuration = 0.5f;
        particalArg.physicalObjectCreateArg.mass = 1.f;
        particalArg.scale = {0.05, 0.05, 0.05};

        GameObject& particleGO = Scene::getSceneUse()->add<GameObject>(Scene::getSceneUse()->getWorld(), GameObjectCreateArg{"ParticleContenerBlood", {rayInfo.intersectionsInfo.intersection1}});
        particleGO.addComponent<ParticuleGenerator>(particalArg);
        particleGO.addComponent<LifeDuration>(3.f);

        ParticleSystemFactory::createDecale(Scene::getSceneUse()->getGameObject("world/DecalContenor"), decaleGOPref, modelDecaleGOPref, rayInfo.intersectionsInfo.normalI1);
        rayInfo.gameObject->destroy();
    }
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

    _orbit.y = fmod(_orbit.y, M_PI * 2);
    _orbit.x = std::clamp(_orbit.x, -M_PI_2f32, M_PI_2f32);
    _gameObject.setRotation({_orbit.x, -_orbit.y, 0.f});

    if (_type == CameraType::FirstPerson)
    {
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

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F2) == 1)
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

void PlayerController::onCollisionEnter(HitInfo& hitInfo)
{
    hitInfo.gameObject = hitInfo.gameObject;
}

void PlayerController::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", "PlayerController"));
    
    nodeParent->append_node(newNode);
}