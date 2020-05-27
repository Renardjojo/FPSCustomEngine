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
    attachedPhysicalObject = static_cast<PhysicalObject*>(_gameObject.getComponent<PhysicalObject>());
    PhysicSystem::addCollider(this);

    for (ScriptComponent* script : refGameObject.getComponents<ScriptComponent>())
    {
        std::cout << script->toString() << std::endl;
        functions.push_back([script](HitInfo& HitInfo){script->onCollisionEnter(HitInfo);});
    }       
}

Collider::Collider (const Collider& other)
    :   Component(*this)
{
    _name = __FUNCTION__;
    attachedPhysicalObject = other.attachedPhysicalObject;
    PhysicSystem::addCollider(this);
}

Collider::Collider (Collider&& other)
    :   Component(*this)
{
    _name = __FUNCTION__;
    attachedPhysicalObject = std::move(other.attachedPhysicalObject);
    PhysicSystem::updateColliderPointor(this, &other);
}

Collider::~Collider ()
{
    PhysicSystem::removeCollider(this);
}