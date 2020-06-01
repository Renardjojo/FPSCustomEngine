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
            Chasing = 2
        };

    private:
        Engine::Physics::PhysicalObject *_physics;
        Engine::Ressources::GameObject* _player{nullptr};
        Engine::Ressources::GameObject* _nexus{nullptr};
        States _state{States::Idle};
        Game::CheckpointManager _checkpointManager;
        float _radius{20.f};
        float _attackRadius{5.f};
        float _speed{25.f};
        float _attackSpeed{1.f};
        float _cooldown{0.f};
        int _damage{1};


        void idle();
        void walk();
        void chasing();
        void autoDestroy();

    public:
        EnnemyController(Engine::Ressources::GameObject &gameObject, Engine::Ressources::GameObject* player, Engine::Ressources::GameObject* nexus, Game::Checkpoint* checkpoint);
        EnnemyController(Engine::Ressources::GameObject &gameObject, const std::vector<std::string>& params);
        virtual ~EnnemyController() = default;

        void start() override;

        void update() override;

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;
        
        void save(xml_document<>& doc, xml_node<>* nodeParent);     
    };
}
#endif // __PLAYERCONTROLLER_HPP__
