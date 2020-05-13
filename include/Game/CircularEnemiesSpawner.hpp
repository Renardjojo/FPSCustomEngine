//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-08 - 17 h 39

#ifndef _CIRCULAR_ENEMIES_SPAWNER_H
#define _CIRCULAR_ENEMIES_SPAWNER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/LowRenderer/model.hpp"

namespace Game
{

    struct EnemiePrefasAndSpawnChance
    {
        Engine::LowRenderer::ModelCreateArg prefab;
    };

    typedef std::vector<EnemiePrefasAndSpawnChance> EnemieInfo;


    class CircularEnemiesSpawner : public Engine::Core::Component::ScriptComponent
    {
        private:
        
        Engine::Core::Maths::Vec3               _spawnPosition          = Engine::Core::Maths::Vec3::zero;
        EnemieInfo                              _enemiePrefas;
        float                                   _zoneRadius             = 3.f;  /*in sec*/
        float                                   _spawnDelay             = 1.f;  /*in sec*/
        float                                   _spawnDelayInterval     = 0.f; /*in sec*/
        float                                   _delayCount             = 0.f;
        float                                   _nextDelay              = _spawnDelay;

        
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
        CircularEnemiesSpawner(Engine::Ressources::GameObject &gameObject, const EnemieInfo& enemisPrefabs, Engine::Core::Maths::Vec3& spawnPosition, float zoneRadius, float spawnDelay, float spawnDelayInterval = 0.f)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _enemiePrefas                               {enemisPrefabs},
                _spawnPosition                              {spawnPosition},
                _zoneRadius                                 {zoneRadius},
                _spawnDelay                                 {spawnDelay},
                _spawnDelayInterval                         {spawnDelayInterval}, 
                _delayCount                                 {0.f},
                _nextDelay                                  {_spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval)}
        {}

        ~CircularEnemiesSpawner() = default;

        void update() override
        {   
            if (_enemiePrefas.empty())
                return;

            _delayCount += Engine::Core::System::TimeSystem::getDeltaTime();

            while (_delayCount >= _nextDelay)
            {
                _delayCount -= _nextDelay;
                _nextDelay   = _spawnDelay + Engine::Core::Maths::Random::ranged(-_spawnDelayInterval, _spawnDelayInterval);

                Engine::LowRenderer::ModelCreateArg enemiePrefasCopy = _enemiePrefas[Engine::Core::Maths::Random::ranged<int>(_enemiePrefas.size())].prefab;
                Engine::Core::Maths::Vec2 newPosition = Engine::Core::Maths::Random::peripheralCircularCoordinate(Engine::Core::Maths::Vec2{enemiePrefasCopy.position.x, enemiePrefasCopy.position.y}, _zoneRadius);
                enemiePrefasCopy.position.x = newPosition.x;
                enemiePrefasCopy.position.y = newPosition.y;

                gameObject.addChild<Engine::LowRenderer::Model>(enemiePrefasCopy);
            }
        }
    };

} //namespace Game

#endif //_CIRCULAR_ENEMIES_SPAWNER_H