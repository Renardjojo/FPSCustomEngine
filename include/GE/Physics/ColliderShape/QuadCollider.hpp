//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 09 h 51

#ifndef _QUAD_COLLIDER_H
#define _QUAD_COLLIDER_H

#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Core/Maths/Shape3D/Quad.hpp"

namespace Engine::Physics::ColliderShape
{
    class QuadCollider : public Collider
    {

    public:

        QuadCollider (Engine::Ressources::GameObject& refGameObject)
            :   Collider        (refGameObject),
                quad_          ()
        {
            _name = __FUNCTION__;
        }

        QuadCollider (const QuadCollider& other)
            :   Collider    (*this),
                quad_       (other.quad_)
        {
            _name = __FUNCTION__;
        }

        QuadCollider (QuadCollider&& other)
            :   Collider        (*this),
                quad_        (std::move(other.quad_))
        {
            _name = __FUNCTION__;
        }

        QuadCollider() = delete;
        virtual ~QuadCollider() = default;
        
        Engine::Core::Maths::Shape3D::Quad        getGlobalQuad() const noexcept
        { 
            const Engine::Core::Maths::Referential globalReferential {  quad_.getReferential().origin + _gameObject.getPosition(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorRight()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorUp()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorForward()).normalize()};

            const float extIGlobal = quad_.getExtI() + _gameObject.getScale().x / 2.f;
            const float extJGlobal = quad_.getExtJ() + _gameObject.getScale().y / 2.f;
            
            return Engine::Core::Maths::Shape3D::Quad{globalReferential, extIGlobal, extJGlobal};
        }
        
        const Engine::Core::Maths::Shape3D::Quad& getLocalQuad()  const noexcept
        { 
            return quad_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            
            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::Quad  quad_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_QUAD_COLLIDER_H