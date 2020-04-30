//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "GE/Ressources/Component.hpp"
#include "GE/Physics/PhysicalObject.hpp"

namespace Engine::Physics
{

class Collider : public Engine::Ressources::Component
{
public:

    Collider(Engine::Ressources::GameObject &refGameObject);
    Collider(const Collider &other);
    Collider(Collider &&other);
    virtual ~Collider();
    
    Collider() = delete;

    virtual bool isCollidingWith(Collider& collider);

    virtual bool IsCollidingWithSphere();
    virtual bool IsCollidingWithBox();


    PhysicalObject* GetAttachedPhysicalObject() { return attachedPhysicalObject; }

    float GetBounciness() { return bounciness; }
    float SetBounciness(float _bounciness) { bounciness = _bounciness; }

private:

    PhysicalObject* attachedPhysicalObject;

    float bounciness;
    float friction;

};


} /*namespace */

#endif //_COLLIDER_H