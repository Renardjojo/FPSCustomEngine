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

        Loot(Engine::Ressources::GameObject &gameObject);

        Loot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~Loot() = default;

        void start () noexcept override;

        void update () noexcept override;

        virtual void effect(){}

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_LOOT_H