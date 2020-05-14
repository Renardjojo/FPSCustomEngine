//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _INFINITE_CYLINDER_COLLIDER_H
#define _INFINITE_CYLINDER_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"

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

    class InfiniteCylinderCollider : public Collider
    {

    public:

        InfiniteCylinderCollider (Engine::Ressources::GameObject& refGameObject, EPositiveDirectionnalAxes directionAxe = EPositiveDirectionnalAxes::ForwardAxe)
            :   Collider        (refGameObject),
            InfiniteCylinder_        (),
                directionAxe_   (directionAxe)
        {}

        InfiniteCylinderCollider (const InfiniteCylinderCollider& other)
            :   Collider    (*this),
            InfiniteCylinder_       (other.InfiniteCylinder_),
                directionAxe_   (other.directionAxe_)
        {}

        InfiniteCylinderCollider (InfiniteCylinderCollider&& other)
            :   Collider        (*this),
            InfiniteCylinder_        (std::move(other.InfiniteCylinder_)),
                directionAxe_   (std::move(other.directionAxe_))
        {}

        InfiniteCylinderCollider() = delete;
        virtual ~InfiniteCylinderCollider() = default;
        
        Engine::Core::Maths::Shape3D::InfiniteCylinder        getGlobalInfiniteCylinder() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = gameObject.entity->getPosition() +InfiniteCylinder_.getLine().getOrigin();
            Engine::Core::Maths::Vec3 globalDirection =InfiniteCylinder_.getLine().getOrigin();
            float globalRadius =InfiniteCylinder_.getRadius();

            if (directionAxe_ == EPositiveDirectionnalAxes::ForwardAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorForward();
                globalRadius    += (gameObject.entity->getScale().x + gameObject.entity->getScale().y) / 2.f;
            }
            else if (directionAxe_ == EPositiveDirectionnalAxes::RightAxe)
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorRight();
                globalRadius    += (gameObject.entity->getScale().z + gameObject.entity->getScale().y) / 2.f;
            }
            else
            {
                globalDirection += gameObject.entity->getModelMatrix().getVectorUp();
                globalRadius    += (gameObject.entity->getScale().x + gameObject.entity->getScale().z) / 2.f;
            }

            return Engine::Core::Maths::Shape3D::InfiniteCylinder{globalCenter, globalDirection.getNormalize(), globalRadius};
        }
        
        const Engine::Core::Maths::Shape3D::InfiniteCylinder& getLocalInfiniteCylinder()  const noexcept
        { 
            return InfiniteCylinder_;
        }

    private:

        Engine::Core::Maths::Shape3D::InfiniteCylinder  InfiniteCylinder_;
        EPositiveDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_INFINITE_CYLINDER_COLLIDER_H