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

        Vec3            getCenter()     const noexcept final     { return transform_.getPosition() + center_; }
        const Vec3&     getLocalCenter() const noexcept          { return center_; }

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

} /*namespace Engine::Core::Maths::Shape3D::Linked*/

#endif //_LINKED_AABB_H