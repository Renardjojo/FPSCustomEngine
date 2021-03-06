//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-26 - 15 h 16

#ifndef __NEXUS_HPP__
#define __NEXUS_HPP__

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"

namespace Game
{
    class Nexus : public Engine::Core::Component::ScriptComponent
    {

    private:

    Engine::LowRenderer::PointLight* _light;

    int _life{1000};
    int _maxLife {1000};
    float _maxLightIntensity {0.f};

    public:
        Nexus(Engine::Ressources::GameObject &gameObject);
        Nexus(Engine::Ressources::GameObject &gameObject, const std::vector<std::string>& params);

        virtual ~Nexus() = default;

        void start() override;
        void update() override;
        void fixedUpdate() override;

        void inflictDamage(int damage);

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);

    };
    
} // namespace Game

#endif // __NEXUS_HPP__
