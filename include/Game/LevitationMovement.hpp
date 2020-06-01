//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _LEVIATION_MOVEMENT_H
#define _LEVIATION_MOVEMENT_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Game
{
    class LevitationMovement : public Engine::Core::Component::ScriptComponent
    {
        private:

        float                                   _speed              {1.f};
        float                                   _heightIntensity    {1.f};
        float                                   _delay              {0.f};
        float                                   _initialY           {0.f};

        public:

        LevitationMovement(Engine::Ressources::GameObject &gameObject, float speed, float heightIntensity)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _speed                                      {speed},
                _heightIntensity                            {heightIntensity}
        {
            _name = __FUNCTION__;
        }

        LevitationMovement (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _speed                                      {std::stof(params[0])},
                _heightIntensity                            {std::stof(params[1])},
                _delay                                      {std::stof(params[2])}
        {
            _name = __FUNCTION__;
        }

        virtual ~LevitationMovement() = default;

        void start() override
        {
            _initialY = _gameObject.getPosition().y;
        }

        void update() override
        {   
            _delay += Engine::Core::System::TimeSystem::getDeltaTime();

            if(_delay > M_PI * 2.f)
            {
               _delay -= M_PI * 2.f;
            }
            std::cout << _delay << std::endl;
            
            _gameObject.setTranslation({_gameObject.getPosition().x, _initialY + cosf(_delay * _speed) * _heightIntensity, _gameObject.getPosition().z});
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("speed", doc.allocate_string(std::to_string(_speed).c_str())));
            newNode->append_attribute(doc.allocate_attribute("heightIntensity", doc.allocate_string(std::to_string(_heightIntensity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_LEVIATION_MOVEMENT_H