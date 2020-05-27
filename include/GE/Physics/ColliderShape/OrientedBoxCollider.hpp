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
        {
            _name = __FUNCTION__;
        }

        OrientedBoxCollider (const OrientedBoxCollider& other)
            :   Collider     (*this),
                orientedBox_ (other.orientedBox_)
        {
            _name = __FUNCTION__;
        }

        OrientedBoxCollider (OrientedBoxCollider&& other)
            :   Collider        (*this),
                orientedBox_    (std::move(other.orientedBox_))
        {
            _name = __FUNCTION__;
        }

        OrientedBoxCollider (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Collider    (refGameObject),
                orientedBox_     (Engine::Core::Maths::Referential{  {std::stof(params[0]), std::stof(params[1]), std::stof(params[2])},
                                                                    {std::stof(params[3]), std::stof(params[4]), std::stof(params[5])},
                                                                    {std::stof(params[6]), std::stof(params[7]), std::stof(params[8])},
                                                                    {std::stof(params[9]), std::stof(params[10]), std::stof(params[11])}},
                                                                    std::stof(params[12]), std::stof(params[13]), std::stof(params[14]))
        {
            _name = __FUNCTION__;
        }

        OrientedBoxCollider() = delete;

        virtual ~OrientedBoxCollider() = default;
        
        Engine::Core::Maths::Shape3D::OrientedBox        getGlobalOrientedBox() const noexcept
        { 
            const Engine::Core::Maths::Referential globalReferential {  orientedBox_.getReferential().origin + _gameObject.getPosition(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorRight()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorUp()).normalize(),
                                                                        (Engine::Core::Maths::Mat3(_gameObject.getModelMatrix()).getVectorForward()).normalize()};

            const float extIGlobal = orientedBox_.getExtI() + _gameObject.getScale().x / 2.f;
            const float extJGlobal = orientedBox_.getExtJ() + _gameObject.getScale().y / 2.f;
            const float extKGlobal = orientedBox_.getExtK() + _gameObject.getScale().z / 2.f;

            return Engine::Core::Maths::Shape3D::OrientedBox{globalReferential, extIGlobal, extJGlobal, extKGlobal};
        }
        
        const Engine::Core::Maths::Shape3D::OrientedBox& getLocalOrientedBox()  const noexcept
        { 
            return orientedBox_;
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));


            newNode->append_attribute(doc.allocate_attribute("localCenter0", doc.allocate_string(std::to_string(orientedBox_.getReferential().origin.e[0]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("localCenter1", doc.allocate_string(std::to_string(orientedBox_.getReferential().origin.e[1]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("localCenter2", doc.allocate_string(std::to_string(orientedBox_.getReferential().origin.e[2]).c_str()))); 

            newNode->append_attribute(doc.allocate_attribute("unitI0", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitI.e[0]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitI1", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitI.e[1]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitI2", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitI.e[2]).c_str()))); 

            newNode->append_attribute(doc.allocate_attribute("unitJ0", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitJ.e[0]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitJ1", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitJ.e[1]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitJ2", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitJ.e[2]).c_str()))); 

            newNode->append_attribute(doc.allocate_attribute("unitK0", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitK.e[0]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitK1", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitK.e[1]).c_str())));
            newNode->append_attribute(doc.allocate_attribute("unitK2", doc.allocate_string(std::to_string(orientedBox_.getReferential().unitK.e[2]).c_str()))); 

            newNode->append_attribute(doc.allocate_attribute("rightLenght", doc.allocate_string(std::to_string(orientedBox_.getExtI()).c_str())));
            newNode->append_attribute(doc.allocate_attribute("upLenght", doc.allocate_string(std::to_string(orientedBox_.getExtJ()).c_str())));
            newNode->append_attribute(doc.allocate_attribute("forwardLenght", doc.allocate_string(std::to_string(orientedBox_.getExtK()).c_str())));

            nodeParent->append_node(newNode);
        }

    private:

        Engine::Core::Maths::Shape3D::OrientedBox orientedBox_;

    };


} /*namespace Engine::Physics::Collider*/

#endif //_ORIENTED_BOX_COLLIDER_H