//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _COLLIDER_H
#define _COLLIDER_H

#include "GE/Ressources/Component.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include <functional>

namespace Engine::Physics::ColliderShape
{
    struct HitInfo
    {
        Engine::Core::Maths::ShapeRelation::Intersection    intersectionsInfo;
        Engine::Ressources::GameObject*                     gameObject;
        float                                               gameObjectInertia;
    };

    class Collider : public Engine::Ressources::Component
    {
    public:

        Collider(Engine::Ressources::GameObject &refGameObject);
        Collider(Engine::Ressources::GameObject &refGameObject, float bounciness, float friction);
        Collider(const Collider &other);
        Collider(Collider &&other);
        virtual ~Collider();
        
        Collider() = delete;

        //virtual bool isCollidingWith(Collider& collider) { return false; }
        //virtual bool isCollidingWith(OrientedBoxCollider& collider) { return false; }

        virtual void OnCollisionEnter(HitInfo& hitInfo)
        {
            for (std::function<void(HitInfo&)>& function : functions)
                function(hitInfo);
        }

        Engine::Physics::PhysicalObject* GetAttachedPhysicalObject() { return _attachedPhysicalObject; }
        void SetAttachedPhysicalObject(Engine::Physics::PhysicalObject* newPhysicalBody) { _attachedPhysicalObject = newPhysicalBody; }

        float getBounciness() { return _bounciness; }
        void setBounciness(float bounciness) { _bounciness = bounciness; }

        float getFriction() { return _friction; }
        void setFriction(float friction) { _friction = friction; }

        std::vector<std::function<void(HitInfo&)>> functions;
        
    protected:

        Engine::Physics::PhysicalObject* _attachedPhysicalObject;

        float _bounciness{0.5};
        float _friction{1};
    };
} /*namespace Engine::Physics::Collider*/

#endif //_COLLIDER_H