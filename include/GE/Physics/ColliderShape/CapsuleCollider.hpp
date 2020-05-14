//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _CAPSULE_COLLIDER_H
#define _CAPSULE_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Capsule.hpp"

namespace Engine::Physics::ColliderShape
{

#ifndef _DIRECTIONAL_AXES_MESH
#define _DIRECTIONAL_AXES_MESH    
    enum class EPositiveDirectionnalAxes
    {
        RightAxe,
        UpAxe,
        ForwardAxe
    };
#endif //_DIRECTIONAL_AXES_MESH

    class CapsuleCollider : public Collider
    {

    public:

        CapsuleCollider (Engine::Ressources::GameObject& refGameObject, EPositiveDirectionnalAxes directionAxe = EPositiveDirectionnalAxes::ForwardAxe)
            :   Collider        (refGameObject),
                capsule_        (),
                directionAxe_   (directionAxe)
        {}

        CapsuleCollider (const CapsuleCollider& other)
            :   Collider    (*this),
                capsule_       (other.capsule_),
                directionAxe_   (other.directionAxe_)
        {}

        CapsuleCollider (CapsuleCollider&& other)
            :   Collider        (*this),
                capsule_        (std::move(other.capsule_)),
                directionAxe_   (std::move(other.directionAxe_))
        {}

        CapsuleCollider() = delete;
        virtual ~CapsuleCollider() = default;
        
        Engine::Core::Maths::Shape3D::Capsule        getGlobalCapsule() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = gameObject.entity->getPosition() + capsule_.getCenter();
            Engine::Core::Maths::Vec3 globalDirection = capsule_.getCenter();
            float globalHeight = capsule_.getSegment().getLenght();
            float globalRadius = capsule_.getRadius();

            if (directionAxe_ == EPositiveDirectionnalAxes::ForwardAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorForward();
                globalHeight    += gameObject.entity->getScale().z;
                globalRadius    += (gameObject.entity->getScale().x + gameObject.entity->getScale().y) / 2.f;
            }
            else if (directionAxe_ == EPositiveDirectionnalAxes::RightAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorRight();
                globalHeight    += gameObject.entity->getScale().x;
                globalRadius    += (gameObject.entity->getScale().z + gameObject.entity->getScale().y) / 2.f;
            }
            else
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorUp();
                globalHeight    += gameObject.entity->getScale().y;
                globalRadius    += (gameObject.entity->getScale().x + gameObject.entity->getScale().z) / 2.f;
            }

            return Engine::Core::Maths::Shape3D::Capsule{globalCenter, globalDirection.getNormalize(), globalHeight, globalRadius};
        }
        
        const Engine::Core::Maths::Shape3D::Capsule& getLocalCapsule()  const noexcept
        { 
            return capsule_;
        }

    private:

        Engine::Core::Maths::Shape3D::Capsule   capsule_;
        EPositiveDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_CAPSULE_COLLIDER_H