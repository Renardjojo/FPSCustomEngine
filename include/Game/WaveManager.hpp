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
        std::vector<unsigned int> _contenorNumberEnemie {}; // from weakest to strongest
        size_t          _currentWave         {0};
        size_t          _waveOffSet          {0};
        size_t          _waveStepOffSet      {1};
        float           _minSpawnIntervale   {0.2f};
        float           _maxSpawnIntervale   {1.5f};
        float           _nextSpawnEnemie      {0.f};
        float           _delay                {0.f};
        size_t          _numberEnemiesGenerate{0};

        public:

        WaveManager(Engine::Ressources::GameObject &gameObject, const SpawnerPrefabs& spawnerPrefabs, const EnemiesPrefabs& enemiesPrefabs, size_t currentWave = 0, float minSpawnIntervale = 0.2f, float maxSpawnIntervale = 1.5f);

        WaveManager (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~WaveManager() = default;

        void update() override;

        void nextWave();

        void save(xml_document<>& doc, xml_node<>* nodeParent);

        int generateNumberEnnemy (std::vector<unsigned int>& refContenorNumberEnemie);
    };

} //namespace Game

#endif //_WAVE_MANAGER_H