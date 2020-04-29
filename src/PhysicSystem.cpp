#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Engine::Physics;
using namespace Engine::Core::TimeSystem;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
Engine::Core::Maths::Vec3       PhysicSystem::gravity;

void PhysicSystem::update() noexcept
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (object->isKinematic())
            continue;
        /*update movement induct by the differente force on the object*/
        object->getGameObject().entity->translate((object->GetVelocity() / object->GetMass() + gravity) * TimeSystem::getDeltaTime());


        /*check if object collided with an other object*/
    }
}