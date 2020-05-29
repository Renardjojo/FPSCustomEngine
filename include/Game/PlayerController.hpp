#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"

namespace Game
{

    class PlayerController : public Engine::Core::Component::ScriptComponent
    {
        enum class CameraType
        {
            FirstPerson,
            ThirdPerson
        };

    private:
        Engine::Ressources::t_RessourcesManager* _rm;
        Engine::Physics::PhysicalObject *_physics;
        float _mouseSpeed{0.75f};
        float _playerForce{600.f};
        float _jumpForce{450.f};
        float _deceleration{0.80f};
        float _playerMaxSpeed{30.f};
        float _cameraSpeed{10.f};
        float _cameraYoffset{5.f};
        Engine::Core::Maths::Vec2 _orbit{0.f,0.f};
        bool _jump{false};
        bool _isGrounded{false};
        
        CameraType _type{CameraType::FirstPerson};

        Engine::LowRenderer::Camera *_camera;

        Engine::Core::Maths::Vec3 _movement{0.f, 0.f, 0.f};
        Engine::Core::Maths::Vec3 _direction{0.f, 0.f, 0.f};

        Engine::Core::Maths::Vec3 cylindricalCoord(float r, float angle);

        void camera();
        void move();
        void showReticle();
        void shoot();

    public:
        PlayerController(Engine::Ressources::GameObject &gameObject);
        virtual ~PlayerController() = default;

        void setCameraType(CameraType type);
        void toggleCameraType();

        void update() override;
        void start() override;
        void fixedUpdate() override;

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };
} // namespace Game
#endif // __PLAYERCONTROLLER_HPP__
