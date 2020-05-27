//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _AABB_COLLIDER_H
#define _AABB_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"

namespace Engine::Physics::ColliderShape
{
    class AABBCollider : public Collider
    {

    public:

        AABBCollider (Engine::Ressources::GameObject& refGameObject)
            :   Collider        (refGameObject),
                AABB_           ()
        {
            _name = __FUNCTION__;
        }

        AABBCollider (const AABBCollider& other)
            :   Collider    (*this),
                AABB_       (other.AABB_)
        {
            _name = __FUNCTION__;
        }

        AABBCollider (AABBCollider&& other)
            :   Collider        (*this),
                AABB_           (std::move(other.AABB_))
        {
            _name = __FUNCTION__;
        }

        AABBCollider() = delete;
        virtual ~AABBCollider() = default;
        
        Engine::Core::Maths::Shape3D::AABB        getGlobalAABB() const noexcept
        { 
            const Engine::Core::Maths::Vec3 centerGlobal = _gameObject.getPosition() + AABB_.getCenter();
            const float extIGlobal = AABB_.getExtI() + _gameObject.getScale().x / 2.f;
            const float extJGlobal = AABB_.getExtJ() + _gameObject.getScale().y / 2.f;
            const float extKGlobal = AABB_.getExtK() + _gameObject.getScale().z / 2.f;

            return Engine::Core::Maths::Shape3D::AABB{centerGlobal, extIGlobal, extJGlobal, extKGlobal};
        }
        
        const Engine::Core::Maths::Shape3D::AABB& getLocalAABB()  const noexcept
        { 
            return AABB_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            
            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::AABB AABB_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_AABB_COLLIDER_H