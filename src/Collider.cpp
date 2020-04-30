#include "GE/Physics/Collider.hpp"
#include "GE/Physics/PhysicSystem.hpp"

#include "GE/Ressources/Component.hpp"

using namespace Engine::Physics;
using namespace Engine::Ressources;

Collider::Collider (GameObject& refGameObject)
    : Component(refGameObject)
{
    attachedPhysicalObject = static_cast<PhysicalObject*>(gameObject.getComponent<PhysicalObject>());
    PhysicSystem::addCollider(this);
}

Collider::Collider (const Collider& other)
    :   Component(*this)
{
    PhysicSystem::addCollider(this);
}

Collider::Collider (Collider&& other)
    :   Component(*this)
{
    PhysicSystem::updateColliderPointor(this, &other);
}

Collider::~Collider ()
{
    PhysicSystem::removeCollider(this);
}

bool Collider::isCollidingWith(Collider &collider)
{
}

bool Collider::IsCollidingWithSphere()
{
}
bool Collider::IsCollidingWithBox()
{
}