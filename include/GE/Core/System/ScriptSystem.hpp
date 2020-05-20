//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 12 h 57

#include <vector>
#include "GE/Core/Component/ScriptComponent.hpp"

#ifndef __SCRIPTSYSTEM_HPP__
#define __SCRIPTSYSTEM_HPP__

using namespace Engine::Core::Component;

namespace Engine::Core::System
{
    class ScriptSystem
    {
    protected:
        static std::vector<ScriptComponent *> _scripts;

    public:
        ScriptSystem() = delete;
        virtual ~ScriptSystem() = delete;
        ScriptSystem &operator=(ScriptSystem const &) = delete;
        ScriptSystem &operator=(ScriptSystem &&) = delete;

        static void start() noexcept;
        static void update() noexcept;
        static void fixedUpdate() noexcept;

        static void addScript(ScriptComponent *scriptComponent);
        static void updateScript(ScriptComponent *pointer, ScriptComponent *old);
        static void removeScript(ScriptComponent *script);
    };

} // namespace Engine::Core::System

#endif // __SCRIPTSYSTEM_HPP__