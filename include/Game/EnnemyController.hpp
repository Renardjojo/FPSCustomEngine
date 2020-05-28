#ifndef __ENNEMYCONTROLLER_HPP__
#define __ENNEMYCONTROLLER_HPP__


#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "Game/Checkpoint.hpp"

namespace Game
{

    class EnnemyController : public Engine::Core::Component::ScriptComponent
    {
        enum States
        {
            Idle = 0,
            Walking = 1,
            Striking = 2,
            Chasing = 3
        };

    private:
        Engine::Ressources::GameObject* _player;
        Engine::Ressources::GameObject* _nexus{nullptr};
        States _state{States::Walking};
        Game::CheckpointManager _checkpointManager;
        float _radius{5.f};
        float _exclusionRadius{0.6f};
        float _speed{5.f};
        float _attackSpeed{1.f};
        float _cooldown{0.f};
        int _damage{1};


        void idle();
        void walk();
        void chasing();
        void striking();

    public:
        EnnemyController(Engine::Ressources::GameObject &gameObject, Engine::Ressources::GameObject* player, Game::Checkpoint* checkpoint);
        EnnemyController(Engine::Ressources::GameObject &gameObject, const std::vector<std::string>& params);
        virtual ~EnnemyController() = default;

        void update() override;

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;
        
        void save(xml_document<>& doc, xml_node<>* nodeParent);     
    };
}
#endif // __PLAYERCONTROLLER_HPP__
