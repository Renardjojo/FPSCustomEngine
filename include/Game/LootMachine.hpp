//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _LOOT_MACHINE_H
#define _LOOT_MACHINE_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
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
        int         _cost               {500};
        bool        _leverMoveForward   {true};
        bool        _isActivated        {false};

        public:

        LootMachine(Engine::Ressources::GameObject &gameObject);

        LootMachine (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~LootMachine() = default;

        void start () noexcept override;

        void update () noexcept override;

        void activate(Engine::Core::Maths::Vec3 playerPosition, int& money);

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_LOOT_MACHINE_H