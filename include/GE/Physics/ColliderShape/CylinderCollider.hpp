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
        {
            _name = __FUNCTION__;
        }

        CylinderCollider (const CylinderCollider& other)
            :   Collider    (*this),
            cylinder_       (other.cylinder_),
                directionAxe_   (other.directionAxe_)
        {
            _name = __FUNCTION__;
        }

        CylinderCollider (CylinderCollider&& other)
            :   Collider        (*this),
            cylinder_        (std::move(other.cylinder_)),
                directionAxe_   (std::move(other.directionAxe_))
        {
            _name = __FUNCTION__;
        }

        CylinderCollider() = delete;
        virtual ~CylinderCollider() = default;
        
        Engine::Core::Maths::Shape3D::Cylinder        getGlobalCylinder() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = _gameObject.getPosition() +cylinder_.getCenter();
            Engine::Core::Maths::Vec3 globalDirection =cylinder_.getCenter();
            float globalHeight =cylinder_.getSegment().getLenght();
            float globalRadius =cylinder_.getRadius();

            if (directionAxe_ == EPositiveDirectionnalAxes::ForwardAxe)
            {
                globalDirection += _gameObject.getModelMatrix().getVectorForward();
                globalHeight    += _gameObject.getScale().z;
                globalRadius    += (_gameObject.getScale().x + _gameObject.getScale().y) / 2.f;
            }
            else if (directionAxe_ == EPositiveDirectionnalAxes::RightAxe)
            {
                globalDirection += _gameObject.getModelMatrix().getVectorRight();
                globalHeight    += _gameObject.getScale().x;
                globalRadius    += (_gameObject.getScale().z + _gameObject.getScale().y) / 2.f;
            }
            else
            {
                globalDirection += _gameObject.getModelMatrix().getVectorUp();
                globalHeight    += _gameObject.getScale().y;
                globalRadius    += (_gameObject.getScale().x + _gameObject.getScale().z) / 2.f;
            }

            return Engine::Core::Maths::Shape3D::Cylinder{globalCenter, globalDirection.getNormalize(), globalHeight, globalRadius};
        }
        
        const Engine::Core::Maths::Shape3D::Cylinder& getLocalCylinder()  const noexcept
        { 
            return cylinder_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            
            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::Cylinder  cylinder_;
        EPositiveDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_CYLINDER_COLLIDER_H