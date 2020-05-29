//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-28 - 11 h 03

#ifndef _WAVE_MANAGER_H
#define _WAVE_MANAGER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Debug/assert.hpp"

#include <vector>
#include <string>

namespace Game
{
    typedef std::vector<std::string> SpawnerPrefabs;
    typedef std::vector<std::string> EnemiesPrefabs;

    class WaveManager : public Engine::Core::Component::ScriptComponent
    {
        private:

        SpawnerPrefabs  _spawnerPrefabs      {};
        EnemiesPrefabs  _enemiesPrefabs      {};
        size_t          _currentWave         {0};
        size_t          _waveOffSet          {0};
        size_t          _waveStepOffSet      {1};
        float           _timeBeforeNextWave  {3.f}; //in second
        float           _delay               {0.f};
        Engine::Ressources::GameObject*     _enemiesContenor     {nullptr};

        public:

        WaveManager(Engine::Ressources::GameObject &gameObject, const SpawnerPrefabs& spawnerPrefabs, const EnemiesPrefabs& enemiesPrefabs, size_t currentWave = 0, size_t waveOffSet = 0, size_t waveStepOffSet = 1, float timeBeforeNextWave = 3.f);

        WaveManager (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~WaveManager() = default;

        void start() override;

        void update() override;

        void nextWave();

        /**
         * @brief return the wave indicator. This wave juste indicate the difficulty step
         * 
         * @return size_t 
         */
        size_t getCurrentWave() const noexcept { return _currentWave; }

        /**
         * @brief Return difficulty of the wave. This number is used to compute the number and type of enemies
         * 
         * @return size_t 
         */
        size_t getWaveDifficulty() const noexcept { return _currentWave * _waveStepOffSet + _waveOffSet;}

        void save(xml_document<>& doc, xml_node<>* nodeParent);

        int generateNumberEnnemy (std::vector<unsigned int>& refContenorNumberEnemie);
    };

} //namespace Game

#endif //_WAVE_MANAGER_H