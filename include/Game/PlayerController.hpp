#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"

namespace Game
{

class PlayerController : public Engine::Core::Component::ScriptComponent
{
    
private:
    float _playerSpeed{10.f};
    float _cameraSpeed{10.f};
    float _cameraYoffset{5.f};
    float _orbity{0.f};
    
    const Engine::Core::InputSystem::Input& _input;
    Engine::LowRenderer::Camera* _camera;

    Engine::Core::Maths::Vec3 _movement{0.f,0.f,0.f};
    Engine::Core::Maths::Vec3 _direction{0.f,0.f,0.f};


    void move();

public:
    PlayerController(Engine::Ressources::GameObject &gameObject,const Engine::Core::InputSystem::Input& input);
    ~PlayerController();

    void update() override;
};
}
#endif // __PLAYERCONTROLLER_HPP__
