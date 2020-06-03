#ifndef __PLAYERCONTROLLER_HPP__
#define __PLAYERCONTROLLER_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include "GE/Ressources/ui.hpp"
#include "Game/Firearm.hpp"

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
        Engine::LowRenderer::SpotLight *_flashLight;
        Engine::Core::Maths::Vec2 _orbit{0.f,0.f};
        float _mouseSpeed{0.5f};
        float _playerForce{30.f};
        float _jumpForce{7.f};
        float _airForce{5.f};
        float _groundForce{30.f};
        float _playerMaxSpeed{1.f};
        float _cameraSpeed{10.f};
        float _cameraYoffset{5.f};
        int   _money{500};
        bool _jump{false};
        bool _isGrounded{false};
        bool _flashLightOn{false};
        int _maxLife{500};
        int _life{500};
        int _points{0};


        std::vector<Firearm*> _firesGuns {}; /*First element is the weapon used*/

        CameraType _type{CameraType::FirstPerson};

        Engine::LowRenderer::Camera *_camera;

        Engine::Core::Maths::Vec3 _movement{0.f, 0.f, 0.f};
        Engine::Core::Maths::Vec3 _direction{0.f, 0.f, 0.f};

        Engine::Core::Maths::Vec3 cylindricalCoord(float r, float angle);

        void camera();
        void move();
        void switchFlashLightState();
        void shoot();
        void switchAimState();

    public:
    
        PlayerController(Engine::Ressources::GameObject &gameObject);
        virtual ~PlayerController() = default;

        
        void update() override;
        void start() override;
        void fixedUpdate() override;

        int* getLife();
        int* getMaxLife();
        int* getPoints();
        int getMoney() { return _money; }
        
        float getMouseSpeed () const noexcept { return _mouseSpeed;}
        void setMouseSpeed (float newSpeed) noexcept { _mouseSpeed = newSpeed;}

        void addMoney(int money) { _money += money; }
        void setMoney(int money) { _money = money; }

        void setCameraType(CameraType type);
        void toggleCameraType();
        void addFirearm(Firearm* Firearm);
        
        void activateLootMachine();
        void inflictDamage(int damage);
        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;
        void save(xml_document<>& doc, xml_node<>* nodeParent);
    };
} // namespace Game
#endif // __PLAYERCONTROLLER_HPP__
