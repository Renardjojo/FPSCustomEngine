//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _ORIENTED_BOX_COLLIDER_H
#define _ORIENTED_BOX_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"
#include "GE/Core/Maths/Referential.hpp"
#include "GE/Core/Maths/mat.hpp"

namespace Engine::Physics::ColliderShape
{
    class OrientedBoxCollider : public Collider
    {

    public:

        OrientedBoxCollider (Engine::Ressources::GameObject& refGameObject)
            :   Collider        (refGameObject),
                orientedBox_    ()
        {}

        OrientedBoxCollider (const OrientedBoxCollider& other)
            :   Collider     (*this),
                orientedBox_ (other.orientedBox_)
        {}

        OrientedBoxCollider (OrientedBoxCollider&& other)
            :   Collider        (*this),
                orientedBox_    (std::move(other.orientedBox_))
        {}

        OrientedBoxCollider() = delete;

        virtual ~OrientedBoxCollider() = default;
        
        Engine::Core::Maths::Shape3D::OrientedBox        getGlobalOrientedBox() const noexcept
        { 
            const Engine::Core::Maths::Referential globalReferential {  orientedBox_.getReferential().origin + gameObject.entity->getPosition(),
                                                                        (Engine::Core::Maths::Mat3(gameObject.entity->getModelMatrix()).getVectorRight()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(gameObject.entity->getModelMatrix()).getVectorUp()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(gameObject.entity->getModelMatrix()).getVectorForward()).normalize()};

            const float extIGlobal = orientedBox_.getExtI() + gameObject.entity->getScale().x / 2.f;
            const float extJGlobal = orientedBox_.getExtJ() + gameObject.entity->getScale().y / 2.f;
            const float extKGlobal = orientedBox_.getExtK() + gameObject.entity->getScale().z / 2.f;

            return Engine::Core::Maths::Shape3D::OrientedBox{globalReferential, extIGlobal, extJGlobal, extKGlobal};
        }
        
        const Engine::Core::Maths::Shape3D::OrientedBox& getLocalOrientedBox()  const noexcept
        { 
            return orientedBox_;
        }

    private:

        Engine::Core::Maths::Shape3D::OrientedBox orientedBox_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_ORIENTED_BOX_COLLIDER_H