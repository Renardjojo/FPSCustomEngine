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

namespace Game
{
    class Nexus : public Engine::Core::Component::ScriptComponent
    {

    private:

    int _life;

    public:
        Nexus(Engine::Ressources::GameObject &gameObject);
        Nexus(Engine::Ressources::GameObject &gameObject, const std::vector<std::string>& params);

        virtual ~Nexus() = default;

        void start() override;
        void update() override;
        void fixedUpdate() override;

        void InflictDamage(int damage) { _life -= damage; }

        void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo) override;

        void save(xml_document<>& doc, xml_node<>* nodeParent);

    };
    
} // namespace Game

#endif // __NEXUS_HPP__
