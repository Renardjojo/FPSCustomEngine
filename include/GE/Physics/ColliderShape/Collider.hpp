//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "GE/Ressources/Component.hpp"
#include "GE/Physics/PhysicalObject.hpp"

namespace Engine::Physics::ColliderShape
{
    class Collider : public Engine::Ressources::Component
    {
    public:

        Collider(Engine::Ressources::GameObject &refGameObject);
        Collider(const Collider &other);
        Collider(Collider &&other);
        virtual ~Collider();
        
        Collider() = delete;

        //virtual bool isCollidingWith(Collider& collider) { return false; }
        //virtual bool isCollidingWith(OrientedBoxCollider& collider) { return false; }

        Engine::Physics::PhysicalObject* GetAttachedPhysicalObject() { return attachedPhysicalObject; }
        void SetAttachedPhysicalObject(Engine::Physics::PhysicalObject* newPhysicalBody) { attachedPhysicalObject = newPhysicalBody; }

        float GetBounciness() { return bounciness_; }
        void SetBounciness(float bounciness) { bounciness_ = bounciness; }

    protected:

        Engine::Physics::PhysicalObject* attachedPhysicalObject;

        float bounciness_{0.5};
        float friction_{0};
    };
} /*namespace Engine::Physics::Collider*/

#endif //_COLLIDER_H