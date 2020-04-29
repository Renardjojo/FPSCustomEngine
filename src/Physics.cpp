#include "GE/Collision&Physics/Physics.hpp"

using namespace Engine::Physics;

Physic::Physic ()
{

}

Physic::Physic (const Physic& other)
{

}

Physic::Physic (Physic&& other)
{

}

Physic::~Physic ()
{

}

Physic& Physic::operator=(Physic const&)
{

}

Physic& Physic::operator=(Physic &&)
{

}

void Physic::update()
{
    for (PhysicalObject* object : pPhysicalObjects)
    {
        // object->translate((object->GetVelocity() / object->GetMass() + gravity) * deltaTime);
        
        // detect collision
    }
}