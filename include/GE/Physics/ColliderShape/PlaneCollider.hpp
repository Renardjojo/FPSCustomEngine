//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _PLANE_COLLIDER_H
#define _PLANE_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"

namespace Engine::Physics::ColliderShape
{
    enum class EDirectionnalAxes
    {
        RightAxe,
        UpAxe,
        ForwardAxe,
        LefttAxe,
        BottomAxe,
        BackwardAxe
    };

    class PlaneCollider : public Collider
    {

    public:

        PlaneCollider (Engine::Ressources::GameObject& refGameObject, EDirectionnalAxes directionAxe = EDirectionnalAxes::ForwardAxe)
            :   Collider        (refGameObject),
                plane_          (),
                directionAxe_   (directionAxe)
        {}

        PlaneCollider (const PlaneCollider& other)
            :   Collider    (*this),
                plane_       (other.plane_),
                directionAxe_   (other.directionAxe_)
        {}

        PlaneCollider (PlaneCollider&& other)
            :   Collider        (*this),
                plane_        (std::move(other.plane_)),
                directionAxe_   (std::move(other.directionAxe_))
        {}

        PlaneCollider() = delete;
        virtual ~PlaneCollider() = default;
        
        Engine::Core::Maths::Shape3D::Plane        getGlobalPlane() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = gameObject.entity->getPosition() + plane_.getNormal() * plane_.getDistance();
            Engine::Core::Maths::Vec3 globalDirection = plane_.getNormal();

            if (directionAxe_ == EDirectionnalAxes::ForwardAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorForward();
            }
            else if (directionAxe_ == EDirectionnalAxes::RightAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorRight();
            }
            else if (directionAxe_ == EDirectionnalAxes::UpAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorUp();
            }
            else if (directionAxe_ == EDirectionnalAxes::BackwardAxe)
            {
                globalDirection -= gameObject.entity->getModelMatrix().getVectorForward();
            }
            else if (directionAxe_ == EDirectionnalAxes::LefttAxe)
            {
                globalDirection -= gameObject.entity->getModelMatrix().getVectorRight();
            }
            else
            {
                globalDirection -= gameObject.entity->getModelMatrix().getVectorUp();
            }
            
            return Engine::Core::Maths::Shape3D::Plane{globalCenter, globalDirection.getNormalize()};
        }
        
        const Engine::Core::Maths::Shape3D::Plane& getLocalPlane()  const noexcept
        { 
            return plane_;
        }

    private:

        Engine::Core::Maths::Shape3D::Plane  plane_;
        EDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_PLANE_COLLIDER_H