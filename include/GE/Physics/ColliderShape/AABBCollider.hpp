//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _AABB_COLLIDER_H
#define _AABB_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"

namespace Engine::Physics::ColliderShape
{
    class AABBCollider : public Collider
    {

    public:

        AABBCollider (Engine::Ressources::GameObject& refGameObject)
            :   Collider        (refGameObject),
                AABB_           ()
        {}

        AABBCollider (const AABBCollider& other)
            :   Collider    (*this),
                AABB_       (other.AABB_)
        {}

        AABBCollider (AABBCollider&& other)
            :   Collider        (*this),
                AABB_           (std::move(other.AABB_))
        {}

        AABBCollider() = delete;
        virtual ~AABBCollider() = default;
        
        Engine::Core::Maths::Shape3D::AABB        getGlobalAABB() const noexcept
        { 
            const Engine::Core::Maths::Vec3 centerGlobal = gameObject.entity->getPosition() + AABB_.getCenter();
            const float extIGlobal = AABB_.getExtI() + gameObject.entity->getScale().x / 2.f;
            const float extJGlobal = AABB_.getExtJ() + gameObject.entity->getScale().y / 2.f;
            const float extKGlobal = AABB_.getExtK() + gameObject.entity->getScale().z / 2.f;

            return Engine::Core::Maths::Shape3D::AABB{centerGlobal, extIGlobal, extJGlobal, extKGlobal};
        }
        
        const Engine::Core::Maths::Shape3D::AABB& getLocalAABB()  const noexcept
        { 
            return AABB_;
        }

    private:

        Engine::Core::Maths::Shape3D::AABB AABB_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_AABB_COLLIDER_H