//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _LIFE_DURATION_H
#define _LIFE_DURATION_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Game
{
    class LifeDuration : public Engine::Core::Component::ScriptComponent
    {
        private:

        float                                   _lifeMaxDelay          {0.f};
        float                                   _delayCount            {0.f};
        bool                                    _useScaledTime         {true};

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
        LifeDuration(Engine::Ressources::GameObject &gameObject, float lifeMaxDelay, bool useScaledTime = true)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _lifeMaxDelay                               {lifeMaxDelay},
                _delayCount                                 {0.f},
                _useScaledTime                              {useScaledTime}
        {}

        virtual ~LifeDuration() = default;

        void update() override
        {   
            _delayCount += _useScaledTime ? Engine::Core::System::TimeSystem::getDeltaTime() : Engine::Core::System::TimeSystem::getUnscaledDetlaTime();

            if(_delayCount >= _lifeMaxDelay)
            {
               _gameObject.destroy();
            }
        }
    };

} //namespace Game

#endif //_LIFE_DURATION_H