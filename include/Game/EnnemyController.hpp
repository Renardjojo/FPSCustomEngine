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
        float _radius{35.f};
        float _attackRadius{12.f};
        float _speed{25.f};
        float _attackSpeed{1.f};
        float _cooldown{0.f};
        int _damage{1};
        int _life{2};
        int _maxLife{2};
        int _valueOnHit{5};
        int _valueOnDeath{50};
        float _exclusionRadius{11.f};


        void idle();
        void walk();
        void chasing();
        void autoDestroy();

    public:
        EnnemyController(Engine::Ressources::GameObject &gameObject, Engine::Ressources::GameObject* player, Engine::Ressources::GameObject* nexus);
        EnnemyController(Engine::Ressources::GameObject &gameObject, const std::vector<std::string>& params);
        virtual ~EnnemyController() = default;

        void start() override;

        void update() override;

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;
        
        void inflictDamage(int damage) { _life -= damage; }

        void setCheckpoint(Game::Checkpoint* checkpoint);
        void setRadius(float radius){ _radius = radius; }
        void setAttackRadius(float attackRadius){ _attackRadius = attackRadius; }
        void setSpeed(float speed){ _speed = speed; }
        void setAttackSpeed(float attackSpeed){ _attackSpeed = attackSpeed; }
        void setCooldown(float cooldown){ _cooldown = cooldown; }
        void setDamage(int damage){ _damage = damage; }
        void setLife(int life){ _life = life; }
        void setValueOnHit(int valueOnHit){ _valueOnHit = valueOnHit; }
        void setValueOnDeath(int valueOnDeath){ _valueOnDeath = valueOnDeath; }

        void save(xml_document<>& doc, xml_node<>* nodeParent);     
    };
}
#endif // __PLAYERCONTROLLER_HPP__
