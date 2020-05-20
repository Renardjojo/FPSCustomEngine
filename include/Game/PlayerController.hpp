#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Physics/PhysicalObject.hpp"

namespace Game
{

    class PlayerController : public Engine::Core::Component::ScriptComponent
    {

    private:
        Engine::Physics::PhysicalObject* _physics;
        float _playerSpeed{10.f};
        float _cameraSpeed{10.f};
        float _cameraYoffset{5.f};
        float _orbity{0.f};

        bool _jump{false};

        Engine::LowRenderer::Camera *_camera;

        Engine::Core::Maths::Vec3 _movement{0.f, 0.f, 0.f};
        Engine::Core::Maths::Vec3 _direction{0.f, 0.f, 0.f};

        Engine::Core::Maths::Vec3 coord(float r, float angle);
        void move();

    public:
        PlayerController(Engine::Ressources::GameObject &gameObject);
        ~PlayerController();

        void update() override;
        void start() override;
        void fixedUpdate() override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };
} // namespace Game
#endif // __PLAYERCONTROLLER_HPP__
