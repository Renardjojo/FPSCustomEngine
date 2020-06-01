#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Physics/PhysicSystem.hpp"

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/Component.hpp"

using namespace Engine::Core::Component;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Ressources;

Collider::Collider (GameObject& refGameObject)
    : Component(refGameObject)
{
    _name = __FUNCTION__;
    _attachedPhysicalObject = static_cast<PhysicalObject*>(_gameObject.getComponent<PhysicalObject>());
    PhysicSystem::addCollider(this);
    for (ScriptComponent* script : refGameObject.getComponents<ScriptComponent>())
    {
        functions.push_back([script](HitInfo& HitInfo){script->onCollisionEnter(HitInfo);});
    }       
}

Collider::Collider (GameObject& refGameObject, float bounciness, float friction)
    : Component(refGameObject)
{
    _name = __FUNCTION__;
    _attachedPhysicalObject = static_cast<PhysicalObject*>(_gameObject.getComponent<PhysicalObject>());
    PhysicSystem::addCollider(this);
    std::cout << refGameObject.getName() << std::endl;
    for (ScriptComponent* script : refGameObject.getComponents<ScriptComponent>())
    {
        std::cout << script->toString() << std::endl;
        functions.push_back([script](HitInfo& HitInfo){script->onCollisionEnter(HitInfo);});

    }       

    _bounciness = bounciness;
    _friction = friction;
}

Collider::Collider (const Collider& other)
    :   Component(*this)
{
    _name = __FUNCTION__;
    _attachedPhysicalObject = other._attachedPhysicalObject;
    _bounciness = other._bounciness;
    PhysicSystem::addCollider(this);
}

Collider::Collider (Collider&& other)
    :   Component(*this)
{
    _name = __FUNCTION__;
    _attachedPhysicalObject = std::move(other._attachedPhysicalObject);
    _bounciness = std::move(other._bounciness);
    PhysicSystem::updateColliderPointor(this, &other);
}

Collider::~Collider ()
{
    PhysicSystem::removeCollider(this);
}