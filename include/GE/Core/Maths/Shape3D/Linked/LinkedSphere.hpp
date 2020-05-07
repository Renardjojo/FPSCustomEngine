//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 15 h 09

#ifndef _LINKED_SPHERE_H
#define _LINKED_SPHERE_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"
#include "GE/Physics/transform.hpp"

namespace Engine::Core::Maths::Shape3D::Linked
{
    class LinkedSphere : Sphere
    {
        public :

        explicit LinkedSphere (const Engine::Physics::Transform& transform, float radius = 0.f, const Vec3& localCenter = Vec3::zero)
            :   Sphere              (radius, localCenter),
                transform_          {transform}
        {}

        Vec3            getCenter()         const noexcept override     { return transform_.getPosition() + center_;}
        const Vec3&     getLocalCenter()    const noexcept              { return center_;}
        // set local

        float           getRadius        ()             const noexcept override  { return transform_.getScale().x + radius_;}
        const float&    getLocalRadius   (float radius) const noexcept           { return radius_;}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };
} /*namespace Engine::Core::Maths::Shape3D::Linked*/

#endif //_LINKED_SPHERE_H