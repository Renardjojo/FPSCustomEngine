//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef __SCRIPTCOMPONENT_HPP__
#define __SCRIPTCOMPONENT_HPP__

#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/Component.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include <functional>

namespace Engine::Core::Component
{
    class ScriptComponent : public Engine::Ressources::Component
    {
    private:
    public:
        ScriptComponent(Engine::Ressources::GameObject &gameobject);
        ScriptComponent(const ScriptComponent &other) = default;
        ScriptComponent(ScriptComponent &&other) = default;
        virtual ~ScriptComponent();

        ScriptComponent() = delete;
        ScriptComponent &operator=(ScriptComponent const &other) = delete;
        ScriptComponent &operator=(ScriptComponent &&other) = delete;

        virtual void start(){};
        virtual void fixedUpdate(){};
        virtual void update(){};

        virtual void onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo);

        //Engine::Ressources::GameObject& getGameObject () { return gameObject;}
    };
} // namespace Engine::Core::Component

#endif // __SCRIPTCOMPONENT_HPP__