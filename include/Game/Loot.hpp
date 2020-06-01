//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _LOOT_H
#define _LOOT_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/scene.hpp"
#include "Game/CircularEntitiesSpawner.hpp"

namespace Game
{
    class Loot : public Engine::Core::Component::ScriptComponent
    {
        protected :

        Engine::Ressources::GameObject* _playerGO           {nullptr};
        float                           _activationRadius   {8.f};

        public:

        Loot(Engine::Ressources::GameObject &gameObject)
            :   Engine::Core::Component::ScriptComponent    {gameObject}
        {
            _name = __FUNCTION__;
        }

        Loot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _activationRadius                           {std::stof(params[0])}
        {
            _name = __FUNCTION__;
        }

        virtual ~Loot() = default;

        void start () noexcept override
        {
            _playerGO = &Engine::Ressources::Scene::getCurrentScene()->getGameObject("Players/Player1");
        }

        void update () noexcept override
        {
            std::cout << (_playerGO->getGlobalPosition() - _gameObject.getGlobalPosition()).length() << "   " <<  _activationRadius << std::endl;
            if ((_playerGO->getGlobalPosition() - _gameObject.getGlobalPosition()).length() <= _activationRadius)
            {
                effect();
                _gameObject.destroy();
            }
        }

        virtual void effect()
        {}

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("activationRadius", doc.allocate_string(std::to_string(_activationRadius).c_str())));
            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_LOOT_H