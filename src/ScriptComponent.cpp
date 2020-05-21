#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/ScriptSystem.hpp"

using namespace Engine::Core::Component;
using namespace Engine::Ressources;
using namespace Engine::Core::System;
using namespace Engine::Physics::ColliderShape;

ScriptComponent::ScriptComponent (GameObject& _gameobject):Component{_gameobject}
{
    ScriptSystem::addScript(this);
    if (_gameObject.getComponent<Collider>())
        _gameObject.getComponent<Collider>()->functions.push_back([&](HitInfo& HitInfo){onCollisionEnter(HitInfo);});

}

ScriptComponent::~ScriptComponent()
{
    ScriptSystem::removeScript(this);
}

void ScriptComponent::onCollisionEnter(Engine::Physics::ColliderShape::HitInfo& hitInfo)
{

}
