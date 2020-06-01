//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _MAX_ELEMENT_CONTENOR_H
#define _MAX_ELEMENT_CONTENOR_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Game
{
    class MaxElementConteneur : public Engine::Core::Component::ScriptComponent
    {
        private:

        unsigned int                                   _maxElement          {10};

        public:

        MaxElementConteneur(Engine::Ressources::GameObject &gameObject, unsigned int maxElement)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _maxElement                                 {maxElement}
        {
            _name = __FUNCTION__;
        }

        MaxElementConteneur (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _maxElement                                 {static_cast<unsigned int>(std::stoi(params[0]))}
        {
            _name = __FUNCTION__;
        }

        virtual ~MaxElementConteneur() = default;

        void update() override
        {   
            while(_gameObject.children.size() > _maxElement)
            {
                _gameObject.destroyChild(_gameObject.children.front().get());
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("maxElement", doc.allocate_string(std::to_string(_maxElement).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_MAX_ELEMENT_CONTENOR_H