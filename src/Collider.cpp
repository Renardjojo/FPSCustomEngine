#include "GE/Physics/Collider.hpp"
#include "GE/Core/Maths/ShapeRelation/SphereOrientedBox.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"
#include "GE/Physics/PhysicSystem.hpp"

#include "GE/Ressources/Component.hpp"

using namespace Engine::Physics;
using namespace Engine::Ressources;
using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::ShapeRelation;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::Shape3D::Linked;

Collider::Collider (GameObject& refGameObject)
    : Component(refGameObject)
{
    attachedPhysicalObject = static_cast<PhysicalObject*>(gameObject.getComponent<PhysicalObject>());
    PhysicSystem::addCollider(this);
}

Collider::Collider (const Collider& other)
    :   Component(*this)
{
    attachedPhysicalObject = other.attachedPhysicalObject;
    PhysicSystem::addCollider(this);
}

Collider::Collider (Collider&& other)
    :   Component(*this)
{
    attachedPhysicalObject = std::move(other.attachedPhysicalObject);
    PhysicSystem::updateColliderPointor(this, &other);
}

Collider::~Collider ()
{
    PhysicSystem::removeCollider(this);
}

SphereCollider::SphereCollider (GameObject& refGameObject)
    : Collider(refGameObject),
    sphere_(*refGameObject.entity)
{
}

SphereCollider::SphereCollider (const SphereCollider& other)
    : Collider(*this),
    sphere_(other.sphere_)
{
}

SphereCollider::SphereCollider (SphereCollider&& other)
    : Collider(*this),
    sphere_(std::move(other.sphere_))
{   
}

bool SphereCollider::isCollidingWith(Collider& collider)
{
    Intersection intersection;

    OrientedBoxCollider* boxCollider = dynamic_cast<OrientedBoxCollider*>(&collider);

    if (boxCollider)
        return SphereOrientedBox::isSphereOrientedBoxCollided(sphere_.getGlobalSphere(), boxCollider->GetBox(), intersection);
    
    return false;
}

// bool SphereCollider::isCollidingWith(OrientedBoxCollider& collider)
// {
//     CollisionPoints intersection;

//     return Collision::isSphereOrientedBoxCollided(sphere_, collider.GetBox(), intersection);

// }

SphereCollider::~SphereCollider ()
{
    PhysicSystem::removeCollider(this);
}

OrientedBoxCollider::OrientedBoxCollider (GameObject& refGameObject)
    : Collider(refGameObject),
    orientedbox_(*refGameObject.entity)
{
}

OrientedBoxCollider::OrientedBoxCollider (const OrientedBoxCollider& other)
    : Collider(*this),
    orientedbox_(other.orientedbox_)
{
}

OrientedBoxCollider::OrientedBoxCollider (OrientedBoxCollider&& other)
    : Collider(*this),
    orientedbox_(std::move(other.orientedbox_))
{   
}

bool OrientedBoxCollider::isCollidingWith(Collider& collider)
{
    Intersection intersection;

    SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(&collider);
    if (sphereCollider)
        return SphereOrientedBox::isSphereOrientedBoxCollided(sphereCollider->GetSphere().getGlobalSphere(), orientedbox_, intersection);

    return false;

    // OrientedBoxCollider* boxCollider = dynamic_cast<OrientedBoxCollider*>(&collider);
    // if (boxCollider)
    //     return Collision::IsboxboxCollided(orientedbox_, boxCollider->GetBox(), intersection);
}

// bool OrientedBoxCollider::isCollidingWith(OrientedBoxCollider& collider)
// {
//     return false;
// }

OrientedBoxCollider::~OrientedBoxCollider ()
{
    PhysicSystem::removeCollider(this);
}