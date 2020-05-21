//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _SPHERE_COLLIDER_H
#define _SPHERE_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"

namespace Engine::Physics::ColliderShape
{
    class SphereCollider : public Collider
    {

    public:

        SphereCollider (Engine::Ressources::GameObject& refGameObject)
            :   Collider    (refGameObject),
                sphere_     ()
        {}

        SphereCollider (const SphereCollider& other)
            :   Collider    (*this),
                sphere_ (other.sphere_)
        {}

        SphereCollider (SphereCollider&& other)
            : Collider  (*this),
            sphere_     (std::move(other.sphere_))
        {}

        SphereCollider() = delete;
        
        virtual ~SphereCollider() = default;
        
        Engine::Core::Maths::Shape3D::Sphere        getGlobalSphere()  const noexcept
        { 
            return Engine::Core::Maths::Shape3D::Sphere{_gameObject.getScale().x / 2.f + sphere_.getRadius(), _gameObject.getPosition() + sphere_.getCenter()};; 
        }
        
        const Engine::Core::Maths::Shape3D::Sphere& getLocalSphere()  const noexcept
        { 
            return sphere_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", "SphereCollider"));
            
            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::Sphere sphere_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_SPHERE_COLLIDER_H