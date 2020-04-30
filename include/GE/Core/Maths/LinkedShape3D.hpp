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
    class LinkedSphere : Sphere
    {
        public :

        explicit LinkedSphere (const Engine::Physics::Transform& transform, float radius = 0.f, const Vec3& localCenter = Vec3::zero)
            :   Sphere              (radius, localCenter),
                transform_          {transform}
        {}

        Vec3            getCenter() override    { return transform_.getPosition() + center_;}
        const Vec3&     getLocalCenter()        { return center_;}
        // set local

        float   getRadius() override            { return transform_.getScale().x + radius_;}
        float   getLocalRadius (float radius)   { return radius_;}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

    class LinkedOrientedBox : OrientedBox
    {
        public :

        explicit LinkedOrientedBox (const Engine::Physics::Transform& transform, float rightLenght = 0.f, float upLenght = 0.f, float forwardLenght = 0.f, const Vec3& center = Vec3::zero, const Vec3& rotation = Vec3::zero)
            :   OrientedBox          (rightLenght, upLenght, forwardLenght, center, rotation),
                transform_           {transform}
        {}

        Referential       getReferential()        final       
        { 
            return Referential {ref_.origin + transform_.getPosition(),
                                (ref_.unitI + Mat3(transform_.getModelMatrix()).getVectorRight()).normalize(),
                                (ref_.unitJ + Mat3(transform_.getModelMatrix()).getVectorUp()).normalize(),
                                (ref_.unitK + Mat3(transform_.getModelMatrix()).getVectorForward()).normalize()};
        }
        
        float                   getExtensionValueI()    final       { return iI_ + transform_.getScale().x / 2.f;}
        float                   getExtensionValueJ()    final       { return iJ_ + transform_.getScale().y / 2.f;}
        float                   getExtensionValueK()    final       { return iK_ + transform_.getScale().z / 2.f;}
        
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
