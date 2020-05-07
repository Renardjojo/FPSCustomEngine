//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 15 h 09

#ifndef _LINKED_ORIENTED_BOX_H
#define _LINKED_ORIENTED_BOX_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"
#include "GE/Physics/transform.hpp"
#include "GE/Core/Maths/Referential.hpp"

namespace Engine::Core::Maths::Shape3D::Linked
{
    class LinkedOrientedBox : OrientedBox
    {
        public :

        explicit LinkedOrientedBox (const Engine::Physics::Transform& transform, float rightLenght = 0.f, float upLenght = 0.f, float forwardLenght = 0.f, const Vec3& center = Vec3::zero, const Vec3& rotation = Vec3::zero)
            :   OrientedBox          (rightLenght, upLenght, forwardLenght, center, rotation),
                transform_           {transform}
        {}

        Referential       getReferential()     const noexcept final       
        { 
            return Referential {referential_.origin + transform_.getPosition(),
                                (referential_.unitI + Mat3(transform_.getModelMatrix()).getVectorRight()).normalize(),
                                (referential_.unitJ + Mat3(transform_.getModelMatrix()).getVectorUp()).normalize(),
                                (referential_.unitK + Mat3(transform_.getModelMatrix()).getVectorForward()).normalize()};
        }
        
        float                   getExtI()    const noexcept final       { return iI_ + transform_.getScale().x / 2.f;}
        float                   getExtJ()    const noexcept final       { return iJ_ + transform_.getScale().y / 2.f;}
        float                   getExtK()    const noexcept final       { return iK_ + transform_.getScale().z / 2.f;}
        
        const Referential&      getLocalReferential() const noexcept    { return referential_;}
        const float&            getLocalExtI() const noexcept           { return iI_;}
        const float&            getLocalExtJ() const noexcept           { return iJ_;}
        const float&            getLocalExtK() const noexcept           { return iK_;}
 
        void                    setLocalReferential(const Referential& newRef) noexcept  { referential_  = newRef;}
        void                    setLocalExtI(float iI) noexcept              { iI_   = iI;}
        void                    setLocalExtJ(float iJ) noexcept              { iJ_   = iJ;}
        void                    setLocalExtK(float iK) noexcept              { iK_   = iK;}

        protected :

        const Engine::Physics::Transform&     transform_; 
    };

} /*namespace Engine::Core::Maths::Shape3D::Linked*/

#endif //_LINKED_ORIENTED_BOX_H