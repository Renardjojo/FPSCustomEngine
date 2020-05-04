#include <vector>
#include <iostream>

#include "GE/Core/System/ScriptSystem.hpp"
#include "GE/Core/TimeSystem/time.hpp"

using namespace Engine::Core::System;
std::vector<ScriptComponent *> ScriptSystem::_scripts;

void ScriptSystem::update() noexcept
{
    for (ScriptComponent *script : _scripts)
    {
        script->update();
    }
}

void ScriptSystem::addScript(ScriptComponent *scriptComponent)
{
    _scripts.push_back(scriptComponent);
}

void ScriptSystem::updateScript(ScriptComponent *pointer, ScriptComponent *old)
{
    for (std::vector<ScriptComponent *>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
    {
        if ((*it) == old)
        {
            *it = pointer;
            return;
        }
    }
}

void ScriptSystem::removeScript(ScriptComponent *script)
{
    for (std::vector<ScriptComponent *>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
    {
        if ((*it) == script)
        {
            _scripts.erase(it);
            return;
        }
    }
}