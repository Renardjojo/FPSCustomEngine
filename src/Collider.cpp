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

// SphereCollider::SphereCollider (GameObject& refGameObject)
//     : Collider(refGameObject),
//     sphere_(*refGameObject.entity)
// {
// }

// SphereCollider::SphereCollider (const SphereCollider& other)
//     : Collider(*this),
//     sphere_(other.sphere_)
// {
// }

// SphereCollider::SphereCollider (SphereCollider&& other)
//     : Collider(*this),
//     sphere_(std::move(other.sphere_))
// {   
// }

// bool SphereCollider::isCollidingWith(Collider& collider)
// {
//     Intersection intersection;

//     OrientedBoxCollider* boxCollider = dynamic_cast<OrientedBoxCollider*>(&collider);

//     if (boxCollider)
//         return SphereOrientedBox::isSphereOrientedBoxCollided(sphere_.getGlobalSphere(), boxCollider->GetBox(), intersection);
    
//     return false;
// }

// bool SphereCollider::isCollidingWith(OrientedBoxCollider& collider)
// {
//     CollisionPoints intersection;

//     return Collision::isSphereOrientedBoxCollided(sphere_, collider.GetBox(), intersection);

// }

// SphereCollider::~SphereCollider ()
// {
//     PhysicSystem::removeCollider(this);
// }

// OrientedBoxCollider::OrientedBoxCollider (GameObject& refGameObject)
//     : Collider(refGameObject),
//     orientedbox_(*refGameObject.entity)
// {
// }

// OrientedBoxCollider::OrientedBoxCollider (const OrientedBoxCollider& other)
//     : Collider(*this),
//     orientedbox_(other.orientedbox_)
// {
// }

// OrientedBoxCollider::OrientedBoxCollider (OrientedBoxCollider&& other)
//     : Collider(*this),
//     orientedbox_(std::move(other.orientedbox_))
// {   
// }

// bool OrientedBoxCollider::isCollidingWith(Collider& collider)
// {
//     Intersection intersection;

//     SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(&collider);
//     if (sphereCollider)
//         return SphereOrientedBox::isSphereOrientedBoxCollided(sphereCollider->GetSphere().getGlobalSphere(), orientedbox_, intersection);

//     return false;

//     // OrientedBoxCollider* boxCollider = dynamic_cast<OrientedBoxCollider*>(&collider);
//     // if (boxCollider)
//     //     return Collision::IsboxboxCollided(orientedbox_, boxCollider->GetBox(), intersection);
// }

// // bool OrientedBoxCollider::isCollidingWith(OrientedBoxCollider& collider)
// // {
// //     return false;
// // }

// OrientedBoxCollider::~OrientedBoxCollider ()
// {
//     PhysicSystem::removeCollider(this);
// }