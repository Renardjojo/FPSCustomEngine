//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _LOOT_MACHINE_H
#define _LOOT_MACHINE_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "Game/CircularEntitiesSpawner.hpp"

namespace Game
{
    class LootMachine : public Engine::Core::Component::ScriptComponent
    {
        protected :

        Engine::Ressources::GameObject* _leverGO {nullptr};
        CircularEntitiesSpawner*        _spawner {nullptr};
        float       _forwardAnimDelay   {1.f};
        float       _delay              {0.f};
        float       _activationRadius   {20.f};
        bool        _leverMoveForward   {true};
        bool        _isActivated        {false};

        public:

        LootMachine(Engine::Ressources::GameObject &gameObject)
            :   Engine::Core::Component::ScriptComponent    {gameObject}
        {
            _name = __FUNCTION__;
        }

        LootMachine (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _forwardAnimDelay   {std::stof(params[0])},
                _delay              {std::stof(params[1])},
                _activationRadius   {std::stof(params[2])},
                _leverMoveForward   {static_cast<bool>(std::stoi(params[3]))},
                _isActivated        {static_cast<bool>(std::stoi(params[4]))}
        {
            _name = __FUNCTION__;
        }

        virtual ~LootMachine() = default;

        void start () noexcept override
        {
            _leverGO = _gameObject.getChild("Lever");
            _spawner = _gameObject.getChild("Spawner")->getComponent<CircularEntitiesSpawner>();
        }

        void update () noexcept override
        {
            if (_isActivated)
            {
                if (_leverMoveForward)
                {
                    _delay += Engine::Core::System::TimeSystem::getDeltaTime();

                    if (_delay >=_forwardAnimDelay)
                    {
                        _delay = _forwardAnimDelay - (_forwardAnimDelay - _delay);
                        _leverMoveForward = false;

                        _spawner->addEntitiesToSpawner(1, std::string("Lot") + std::to_string(Engine::Core::Maths::Random::ranged<int>(1, 2)));
                    }
                }
                else
                {
                    _delay -= Engine::Core::System::TimeSystem::getDeltaTime();

                    if (_delay <= 0.f)
                    {
                        _delay = 0.f;
                        _leverMoveForward = true;
                        _isActivated = false;
                    }
                }
                _leverGO->setRotation({_delay * static_cast<float>(M_PI_4) / _forwardAnimDelay, 0.f, 0.f});
            }
        }

        void activate(Engine::Core::Maths::Vec3 playerPosition)
        {
            if ((playerPosition - _gameObject.getGlobalPosition()).length() <= _activationRadius)
                _isActivated = true;
        }

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("forwardAnimDelay", doc.allocate_string(std::to_string(_forwardAnimDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("activationRadius", doc.allocate_string(std::to_string(_activationRadius).c_str())));
            newNode->append_attribute(doc.allocate_attribute("leverMoveForward", doc.allocate_string(std::to_string(_leverMoveForward).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isActivated", doc.allocate_string(std::to_string(_isActivated).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_LOOT_MACHINE_H