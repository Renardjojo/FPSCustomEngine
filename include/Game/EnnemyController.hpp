#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"

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
        float _exclusionRadius{0.5f};
        float _speed{10.f};
        GameObject* _player{nullptr};
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
