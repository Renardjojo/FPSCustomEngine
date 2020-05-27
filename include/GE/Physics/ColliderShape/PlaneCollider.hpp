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
        {
            _name = __FUNCTION__;    
        }

        PlaneCollider (const PlaneCollider& other)
            :   Collider    (*this),
                plane_       (other.plane_),
                directionAxe_   (other.directionAxe_)
        {
            _name = __FUNCTION__;    
        }

        PlaneCollider (PlaneCollider&& other)
            :   Collider        (*this),
                plane_        (std::move(other.plane_)),
                directionAxe_   (std::move(other.directionAxe_))
        {
            _name = __FUNCTION__;    
        }

        PlaneCollider() = delete;
        virtual ~PlaneCollider() = default;
        
        Engine::Core::Maths::Shape3D::Plane        getGlobalPlane() const noexcept
        { 
            const Engine::Core::Maths::Vec3 globalCenter = _gameObject.getPosition() + plane_.getNormal() * plane_.getDistance();
            Engine::Core::Maths::Vec3 globalDirection = plane_.getNormal();

            if (directionAxe_ == EDirectionnalAxes::ForwardAxe)
            {
                globalDirection += _gameObject.getModelMatrix().getVectorForward();
            }
            else if (directionAxe_ == EDirectionnalAxes::RightAxe)
            {
                globalDirection += _gameObject.getModelMatrix().getVectorRight();
            }
            else if (directionAxe_ == EDirectionnalAxes::UpAxe)
            {
                globalDirection += _gameObject.getModelMatrix().getVectorUp();
            }
            else if (directionAxe_ == EDirectionnalAxes::BackwardAxe)
            {
                globalDirection -= _gameObject.getModelMatrix().getVectorForward();
            }
            else if (directionAxe_ == EDirectionnalAxes::LefttAxe)
            {
                globalDirection -= _gameObject.getModelMatrix().getVectorRight();
            }
            else
            {
                globalDirection -= _gameObject.getModelMatrix().getVectorUp();
            }
            
            return Engine::Core::Maths::Shape3D::Plane{globalCenter, globalDirection.getNormalize()};
        }
        
        const Engine::Core::Maths::Shape3D::Plane& getLocalPlane()  const noexcept
        { 
            return plane_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            
            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::Plane  plane_;
        EDirectionnalAxes                       directionAxe_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_PLANE_COLLIDER_H