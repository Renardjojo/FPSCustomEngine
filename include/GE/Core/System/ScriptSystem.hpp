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
        public:

#pragma region constructor/destructor

        ScriptSystem ()					                = delete;
        ScriptSystem (const ScriptSystem& other)	    = delete;
        ScriptSystem (ScriptSystem&& other)			    = delete;
        virtual ~ScriptSystem ()				        = delete;
        ScriptSystem& operator=(ScriptSystem const&)	= delete;
        ScriptSystem& operator=(ScriptSystem &&)		= delete;

#pragma endregion //!constructor/destructor

#pragma region methods

        static void update () noexcept;

#pragma endregion //!methods

#pragma region mutator

        /**
         * @brief 
         * 
         * @param ScriptComponent 
         */
        static
        void addScript (ScriptComponent* scriptComponent) noexcept
        {
            _scripts.push_back(scriptComponent);
        }

        static
        void updateScript (ScriptComponent* pointer, ScriptComponent* old) noexcept
        {
            for (std::vector<ScriptComponent*>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
            {
                if ((*it) == old)
                {
                    *it = pointer;
                    return;
                }
            }
        }

        static
        void removeScript (ScriptComponent* script) noexcept
        {
            for (std::vector<ScriptComponent*>::iterator it = _scripts.begin(); it != _scripts.end(); it++)
            {
                if ((*it) == script)
                {
                    _scripts.erase(it);
                    return;
                }
            }
        }

#pragma endregion mutator

        protected:

#pragma region attribut

            static std::vector<ScriptComponent*> _scripts;

#pragma endregion //!attribut

    };

    
}

#endif // __SCRIPTSYSTEM_HPP__