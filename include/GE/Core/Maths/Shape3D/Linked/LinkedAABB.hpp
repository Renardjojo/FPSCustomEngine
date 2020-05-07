//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 15 h 09

#ifndef _LINKED_AABB_H
#define _LINKED_AABB_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"
#include "GE/Physics/transform.hpp"

namespace Engine::Core::Maths::Shape3D::Linked
{
    class LinkedAABB : public AABB
    {
        public :

        explicit LinkedAABB (const Engine::Physics::Transform& transform, float rightLenght, float upLenght, float forwardLenght, const Vec3& localCenter = Vec3::zero)
            :   AABB              {localCenter, rightLenght, upLenght, forwardLenght},
                transform_        {transform}
        {}

        Vec3                    getCenter()     const noexcept final     { return transform_.getPosition() + center_; }
        float                   getExtI()    const noexcept final       { return iI_ + transform_.getScale().x / 2.f;}
        float                   getExtJ()    const noexcept final       { return iJ_ + transform_.getScale().y / 2.f;}
        float                   getExtK()    const noexcept final       { return iK_ + transform_.getScale().z / 2.f;}
        
        const Vec3&             getLocalCenter() const noexcept         { return center_; }
        const float&            getLocalExtI() const noexcept           { return iI_;}
        const float&            getLocalExtJ() const noexcept           { return iJ_;}
        const float&            getLocalExtK() const noexcept           { return iK_;}
 
        void                    setLocalCenter(const Vec3& newCenter) noexcept  { center_  = newCenter;}
        void                    setLocalExtI(float iI) noexcept              { iI_   = iI;}
        void                    setLocalExtJ(float iJ) noexcept              { iJ_   = iJ;}
        void                    setLocalExtK(float iK) noexcept              { iK_   = iK;}

        AABB getGlobalAABB() const noexcept //return the global AABB
        {
            return AABB{getCenter(), getExtI(), getExtJ(), getExtK()};
        }

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

} /*namespace Engine::Core::Maths::Shape3D::Linked*/

#endif //_LINKED_AABB_H