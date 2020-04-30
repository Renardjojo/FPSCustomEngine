#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"

namespace Game
{

class PlayerController : public Engine::Core::Component::ScriptComponent
{
    
private:
    const Engine::Core::InputSystem::Input& _input;
    void move();

public:
    PlayerController(Engine::Ressources::GameObject &gameObject,const Engine::Core::InputSystem::Input& input);
    ~PlayerController()=default;

    void update() override;
};
}
#endif // __PLAYERCONTROLLER_HPP__
