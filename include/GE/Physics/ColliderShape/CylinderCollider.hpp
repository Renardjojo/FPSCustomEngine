//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _CYLINDER_COLLIDER_H
#define _CYLINDER_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Cylinder.hpp"

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

    class CylinderCollider : public Collider
    {

    public:

        CylinderCollider (Engine::Ressources::GameObject& refGameObject, EPositiveDirectionnalAxes directionAxe = EPositiveDirectionnalAxes::ForwardAxe)
            :   Collider        (refGameObject),
            cylinder_        (),
                directionAxe_   (directionAxe)
        {}

        CylinderCollider (const CylinderCollider& other)
            :   Collider    (*this),
            cylinder_       (other.cylinder_),
                directionAxe_   (other.directionAxe_)
        {}

        CylinderCollider (CylinderCollider&& other)
            :   Collider        (*this),
            cylinder_        (std::move(other.cylinder_)),
                directionAxe_   (std::move(other.directionAxe_))
        {}

        CylinderCollider() = delete;
        virtual ~CylinderCollider() = default;
        
        Engine::Core::Maths::Shape3D::Cylinder        getGlobalCylinder() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = gameObject.getPosition() +cylinder_.getCenter();
            Engine::Core::Maths::Vec3 globalDirection =cylinder_.getCenter();
            float globalHeight =cylinder_.getSegment().getLenght();
            float globalRadius =cylinder_.getRadius();

            if (directionAxe_ == EPositiveDirectionnalAxes::ForwardAxe)
            {
                globalDirection += gameObject.getModelMatrix().getVectorForward();
                globalHeight    += gameObject.getScale().z;
                globalRadius    += (gameObject.getScale().x + gameObject.getScale().y) / 2.f;
            }
            else if (directionAxe_ == EPositiveDirectionnalAxes::RightAxe)
            {
                globalDirection += gameObject.getModelMatrix().getVectorRight();
                globalHeight    += gameObject.getScale().x;
                globalRadius    += (gameObject.getScale().z + gameObject.getScale().y) / 2.f;
            }
            else
            {
                globalDirection += gameObject.getModelMatrix().getVectorUp();
                globalHeight    += gameObject.getScale().y;
                globalRadius    += (gameObject.getScale().x + gameObject.getScale().z) / 2.f;
            }

            return Engine::Core::Maths::Shape3D::Cylinder{globalCenter, globalDirection.getNormalize(), globalHeight, globalRadius};
        }
        
        const Engine::Core::Maths::Shape3D::Cylinder& getLocalCylinder()  const noexcept
        { 
            return cylinder_;
        }

    private:

        Engine::Core::Maths::Shape3D::Cylinder  cylinder_;
        EPositiveDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_CYLINDER_COLLIDER_H