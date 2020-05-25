#include <iostream>

#include "GE/Core/System/ScriptSystem.hpp"
#include "GE/Core/System/TimeSystem.hpp"

using namespace Engine::Core::System;
std::list<ScriptComponent *> ScriptSystem::_scripts;

void ScriptSystem::start() noexcept
{
    for (ScriptComponent *script : _scripts)
    {
        script->start();
    }
}

void ScriptSystem::update() noexcept
{
    for (ScriptComponent *script : _scripts)
    {
        if (script != nullptr)
        script->update();
    }
}

void ScriptSystem::fixedUpdate() noexcept
{
    for (ScriptComponent *script : _scripts)
    {
        script->fixedUpdate();
    }
}

void ScriptSystem::addScript(ScriptComponent *scriptComponent)
{
    _scripts.push_back(scriptComponent);
}

void ScriptSystem::updateScript(ScriptComponent *pointer, ScriptComponent *old)
{
    for (std::list<ScriptComponent *>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
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
    for (std::list<ScriptComponent *>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
    {
        if ((*it) == script)
        {
            _scripts.erase(it);
            return;
        }
    }
}