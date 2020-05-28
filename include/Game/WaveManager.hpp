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
        float           _minSpawnIntervale   {0.5f};
        float           _maxSpawnIntervale   {1.f};

        public:

        WaveManager(Engine::Ressources::GameObject &gameObject, const SpawnerPrefabs& spawnerPrefabs, const EnemiesPrefabs& enemiesPrefabs, size_t currentWave = 0, float minSpawnIntervale = 0.5f, float maxSpawnIntervale = 1.f)
            :   Engine::Core::Component::ScriptComponent    {gameObject},
                _spawnerPrefabs                             {spawnerPrefabs},
                _enemiesPrefabs                             {enemiesPrefabs},
                _currentWave                                {currentWave},
                _minSpawnIntervale                          {minSpawnIntervale},
                _maxSpawnIntervale                          {maxSpawnIntervale}
        {
            GE_assert(!(maxSpawnIntervale >= minSpawnIntervale));
            GE_assert(!(minSpawnIntervale < 0.f));
            GE_assert(!(maxSpawnIntervale < 0.f));
            GE_assert(!spawnerPrefabs.empty());
            GE_assert(!enemiesPrefabs.empty());

            _name = __FUNCTION__;
        }

        WaveManager (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _spawnerPrefabs                             {},
                _enemiesPrefabs                             {},
                _currentWave                                {static_cast<size_t>(std::stoi(params[0]))},
                _minSpawnIntervale                          {std::stof(params[1])},
                _maxSpawnIntervale                          {std::stof(params[2])}
        {
            _name = __FUNCTION__;

            size_t count = 3;

            while (params.size() > count && params[count].substr(0, 2) == "#1")
            {
                _spawnerPrefabs.push_back(params[count].substr(2));
                count++;
            }

            while (params.size() > count && params[count].substr(0, 2) == "#2")
            {
                _enemiesPrefabs.push_back(params[count].substr(2));
                count++;
            }
        }

        virtual ~WaveManager() = default;

        void update() override
        {   

        }

        void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

            newNode->append_attribute(doc.allocate_attribute("currentWave", doc.allocate_string(std::to_string(_currentWave).c_str())));
            newNode->append_attribute(doc.allocate_attribute("minSpawnIntervale", doc.allocate_string(std::to_string(_minSpawnIntervale).c_str())));
            newNode->append_attribute(doc.allocate_attribute("maxSpawnIntervale", doc.allocate_string(std::to_string(_maxSpawnIntervale).c_str())));

            int count = 0;
            for (auto &&path : _spawnerPrefabs)
            {
                std::string data = "#";
                data += std::to_string(1);
                data += path;
                newNode->append_attribute(doc.allocate_attribute((std::string("spawnerPrefabs") + std::to_string(count)).c_str(), doc.allocate_string(data.c_str())));
                count++;
            }

            count = 0;
            for (auto &&path : _enemiesPrefabs)
            {
                std::string data = "#";
                data += std::to_string(2);
                data += path;
                newNode->append_attribute(doc.allocate_attribute((std::string("enemiesPrefabs") + std::to_string(count)).c_str(), doc.allocate_string(data.c_str())));
                count++;
            }

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_WAVE_MANAGER_H