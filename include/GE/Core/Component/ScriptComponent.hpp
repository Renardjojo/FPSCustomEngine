//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef __SCRIPTCOMPONENT_HPP__
#define __SCRIPTCOMPONENT_HPP__

#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/Component.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::Core::Component
{
    class ScriptComponent : public Engine::Ressources::Component
    {
        private:
            bool _active;

        public:
            ScriptComponent (Engine::Ressources::GameObject& gameobject):Engine::Ressources::Component(gameObject),_active{true}{}
    //        ScriptComponent (const ScriptComponent& other)=default;
    //        ScriptComponent (ScriptComponent&& other)=default;
     //       virtual ~ScriptComponent ()=0;
            
            ScriptComponent ()                                        = delete;
            ScriptComponent& operator=(ScriptComponent const& other)  = delete;
            ScriptComponent& operator=(ScriptComponent && other)      = delete;

            virtual void update()=0;
            bool isActive(){return _active;}

        //Engine::Ressources::GameObject& getGameObject () { return gameObject;}

    };
}

#endif // __SCRIPTCOMPONENT_HPP__