#include <vector>
#include "GE/Core/System/ScriptSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Engine::Core::System;
std::vector<ScriptComponent*> ScriptSystem::_scripts; 

void ScriptSystem::update() noexcept
{
    for (ScriptComponent* script : _scripts)
    {
        if (!script->isActive())
            continue;
        
        script->update();
    }
}