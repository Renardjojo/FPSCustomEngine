//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _MAX_ELEMENT_CONTENOR_H
#define _MAX_ELEMENT_CONTENOR_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Game
{
    class MaxElementConteneur : public Engine::Core::Component::ScriptComponent
    {
        private:

        unsigned int                                   _maxElement          {10};

        public:

        /**
         * @brief Construct a new Ranged Enemies Spawner object
         * 
         * @param gameObject 
         * @param spawnPosition 
         * @param zoneRadius 
         * @param spawnDelay 
         * @param spawnDelayInterval : spawnDelay will be compute this + or - this intervale.  
         */
        MaxElementConteneur(Engine::Ressources::GameObject &gameObject, unsigned int maxElement)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _maxElement                                 {maxElement}
        {
            _name = __FUNCTION__;
        }

        virtual ~MaxElementConteneur() = default;

        void update() override
        {   
            while(_gameObject.children.size() > _maxElement)
            {
                _gameObject.destroyChild(_gameObject.children.front().get());
            }
        }
    };

} //namespace Game

#endif //_MAX_ELEMENT_CONTENOR_H