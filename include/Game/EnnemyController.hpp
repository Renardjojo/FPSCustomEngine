#ifndef __ENNEMYCONTROLLER_HPP__
#define __ENNEMYCONTROLLER_HPP__


#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"

namespace Game
{

    class EnnemyController : public Engine::Core::Component::ScriptComponent
    {
        enum class States
        {
            Idle,
            Chasing,
        };

    private:
        float _radius{5.f};
        float _exclusionRadius{0.6f};
        float _speed{5.f};
        Engine::Ressources::GameObject* _player;
        States _state{States::Idle};

        void idle();
        void chasing();
    public:
        EnnemyController(Engine::Ressources::GameObject &gameObject,Engine::Ressources::GameObject* player);
        ~EnnemyController();

        void update() override;
    };
}
#endif // __PLAYERCONTROLLER_HPP__
