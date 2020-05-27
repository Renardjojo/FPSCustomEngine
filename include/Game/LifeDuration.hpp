//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _LIFE_DURATION_H
#define _LIFE_DURATION_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Game
{
    class LifeDuration : public Engine::Core::Component::ScriptComponent
    {
        private:

        float                                   _lifeMaxDelay          {0.f};
        float                                   _delayCount            {0.f};
        bool                                    _useScaledTime         {true};

        public:

        /**
         * @brief Construct a new Ranged Enemies Spawner object
         * 
         * @param gameObject 
         * @param spawnPosition 
         * @param zoneRadius 
         * @param spawnDelay 
         * @param spawnDelayInterval : spawnDelay will be compute this + or - this intervale.  
         */
        LifeDuration(Engine::Ressources::GameObject &gameObject, float lifeMaxDelay, bool useScaledTime = true)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _lifeMaxDelay                               {lifeMaxDelay},
                _delayCount                                 {0.f},
                _useScaledTime                              {useScaledTime}
        {
            _name = __FUNCTION__;
        }

        LifeDuration (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _lifeMaxDelay                               {std::stof(params[0])},
                _delayCount                                 {std::stof(params[1])},
                _useScaledTime                              {static_cast<bool>(std::stof(params[2]))}
        {
            _name = __FUNCTION__;
        }

        virtual ~LifeDuration() = default;

        void update() override
        {   
            _delayCount += _useScaledTime ? Engine::Core::System::TimeSystem::getDeltaTime() : Engine::Core::System::TimeSystem::getUnscaledDeltaTime();

            if(_delayCount >= _lifeMaxDelay)
            {
               _gameObject.destroy();
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("lifeMaxDelay", doc.allocate_string(std::to_string(_lifeMaxDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delayCount", doc.allocate_string(std::to_string(_delayCount).c_str())));
            newNode->append_attribute(doc.allocate_attribute("useScaledTime", doc.allocate_string(std::to_string(_useScaledTime).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_LIFE_DURATION_H