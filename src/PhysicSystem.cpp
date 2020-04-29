#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Engine::Physics;
using namespace Engine::Core::Time;

std::vector<PhysicalObject*>    PhysicSystem::pPhysicalObjects;
Engine::Core::Maths::Vec3       PhysicSystem::gravity = {0,-9.81f,0};

void PhysicSystem::update() noexcept
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        if (object->IsKinematic())
            continue;

        if (object->UseGravity())
            object->AddForce(gravity * TimeSystem::getDeltaTime());
        
        /*update movement induct by the differente force on the object*/
        object->getGameObject().entity->translate((object->GetVelocity() / object->GetMass()) * TimeSystem::getDeltaTime());

        /*check if object collided with an other object*/
    }
}