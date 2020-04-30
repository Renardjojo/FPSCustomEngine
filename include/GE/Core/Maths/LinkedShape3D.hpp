//Project : Math
//Editing by Six Jonathan
//Date : 31/01/2020 - 17 h 19

#ifndef _LINKED_PRIMITIVE3D_H
#define _LINKED_PRIMITIVE3D_H

#include <math.h>
#include <limits>
#include <vector>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/plane.hpp"
#include "GE/Core/Maths/Referential.hpp"
#include "GE/Core/Maths/Shape3D.hpp"
#include "GE/Physics/transform.hpp"

namespace Engine::Core::Maths
{
    class LinkedAABB : public AABB
    {
        public :

        explicit LinkedAABB (const Engine::Physics::Transform& transform, float upLenght, float rightLenght, float forwardLenght, const Vec3& center = Vec3::zero,)
            :   AABB              (upLenght, rightLenght, forwardLenght, center),
                transform_        {transform}
        {}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

    class LinkedSphere : Sphere
    {
        public :

        explicit LinkedSphere (const Engine::Physics::Transform& transform, float radius, const Vec3& localCenter = Vec3::zero)
            :   Sphere              (radius, localCenter),
                transform_          {transform}
        {}

        const Vec3&     getCenter() override { return transform_.getPosition() + center_;}
        const Vec3&     getLocalCenter()    { return center_;}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

    class LinkedOrientedBox : OrientedBox
    {
        public :

        explicit LinkedOrientedBox (const Engine::Physics::Transform& transform, float upLenght, float rightLenght, float forwardLenght, const Vec3& center = Vec3::zero, const Vec3& rotation = Vec3::zero)
            :   OrientedBox          (upLenght, rightLenght, forwardLenght, center, rotation),
                transform_           {transform}
        {}

        const Referential&      getReferential()        final       
        { 
            return Referential {ref_.origin + transform_.getPosition(),
                                (ref_.unitI + Mat3(transform_.getModelMatrix()).getVectorRight()).normalize(),
                                (ref_.unitJ + Mat3(transform_.getModelMatrix()).getVectorUp()).normalize(),
                                (ref_.unitK + Mat3(transform_.getModelMatrix()).getVectorForward()).normalize()};
        }
        
        float                   getExtensionValueI()    final       { return iI_ + transform_.getScale().x;}
        float                   getExtensionValueJ()    final       { return iJ_ + transform_.getScale().y;}
        float                   getExtensionValueK()    final       { return iK_ + transform_.getScale().z;}
        
        const Referential&      getLocalReferential()           { return ref_;}
        float                   getLocalExtensionValueI()       { return iI_;}
        float                   getLocalExtensionValueJ()       { return iJ_;}
        float                   getLocalExtensionValueK()       { return iK_;}
 
        void                    setLocalReferential(const Referential& newRef)   { ref_  = newRef;}
        void                    setLocalExtensionValueI(float iI)                { iI_   = iI;}
        void                    setLocalExtensionValueJ(float iJ)                { iJ_   = iJ;}
        void                    etELocalxtensionValueK(float iK)                 { iK_   = iK;}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };
}

#endif // _LINKED_PRIMITIVE3D_H
