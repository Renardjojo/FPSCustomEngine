//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _UPGRADE_BORN_H_
#define _UPGRADE_BORN_H_

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "Game/PlayerController.hpp"

namespace Game
{
    class UpgradeStation
     : public Engine::Core::Component::ScriptComponent
    {
        protected :

        Game::PlayerController*         _player             {nullptr};
        Engine::Ressources::GameObject* _billBoardIndicator {nullptr};
        float                           _activationRadius   {15.f};
        int                             _cost               {500};

        public:

        UpgradeStation
        (Engine::Ressources::GameObject &gameObject, int cost);

        UpgradeStation
         (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~UpgradeStation() = default;

        void start () noexcept override;

        void update () noexcept override;

        virtual void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController* playerStats);

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

    class MunitionCapacityUpgradeStation : public UpgradeStation
    {
    public:
        MunitionCapacityUpgradeStation(Engine::Ressources::GameObject &gameObject, int cost) : UpgradeStation{gameObject, cost} {}
        MunitionCapacityUpgradeStation(Engine::Ressources::GameObject &refGameObject, const std::vector<std::string> &params)
            : UpgradeStation{refGameObject, params} {}

        virtual ~MunitionCapacityUpgradeStation() = default;

        void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController *playerStats) final;
    };

    class DamageUpgradeStation : public UpgradeStation
    {
    public:
        DamageUpgradeStation(Engine::Ressources::GameObject &gameObject, int cost) : UpgradeStation{gameObject, cost} {}
        DamageUpgradeStation(Engine::Ressources::GameObject &refGameObject, const std::vector<std::string> &params)
            : UpgradeStation{refGameObject, params} {}

        virtual ~DamageUpgradeStation() = default;

        void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController *playerStats) final;
    };

    class FireRateUpgradeStation : public UpgradeStation
    {
    public:
        FireRateUpgradeStation(Engine::Ressources::GameObject &gameObject, int cost) : UpgradeStation{gameObject, cost} {}
        FireRateUpgradeStation(Engine::Ressources::GameObject &refGameObject, const std::vector<std::string> &params)
            : UpgradeStation{refGameObject, params} {}

        virtual ~FireRateUpgradeStation() = default;

        void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController *playerStats) final;
    };

    class ReloadTimeUpgradeStation : public UpgradeStation
    {
    public:
        ReloadTimeUpgradeStation(Engine::Ressources::GameObject &gameObject, int cost) : UpgradeStation{gameObject, cost} {}
        ReloadTimeUpgradeStation(Engine::Ressources::GameObject &refGameObject, const std::vector<std::string> &params)
            : UpgradeStation{refGameObject, params} {}

        virtual ~ReloadTimeUpgradeStation() = default;

        void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController *playerStats) final;
    };

    class AutoUpgradeStation : public UpgradeStation
    {
    public:
        AutoUpgradeStation(Engine::Ressources::GameObject &gameObject, int cost) : UpgradeStation{gameObject, cost} {}
        AutoUpgradeStation(Engine::Ressources::GameObject &refGameObject, const std::vector<std::string> &params)
            : UpgradeStation{refGameObject, params} {}

        virtual ~AutoUpgradeStation() = default;

        void activate(Engine::Core::Maths::Vec3 playerPosition, PlayerController *playerStats) final;
    };

} //namespace Game

#endif //_LOOT_MACHINE_H