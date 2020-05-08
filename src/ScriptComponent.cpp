#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/ScriptSystem.hpp"

using namespace Engine::Core::Component;
using namespace Engine::Ressources;
using namespace Engine::Core::System;

ScriptComponent::ScriptComponent (GameObject& _gameobject):Component{_gameobject}
{
    ScriptSystem::addScript(this);
}

ScriptComponent::~ScriptComponent()
{
    ScriptSystem::removeScript(this);
}