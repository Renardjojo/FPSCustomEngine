#include "Game/WaveManager.hpp"

#include "GE/Ressources/Saves.hpp"
#include "GE/Core/Maths/Random.hpp"

#include "Game/CircularEntitiesSpawner.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::Maths;


WaveManager::WaveManager(GameObject &gameObject, const SpawnerPrefabs& spawnerPrefabs, const EnemiesPrefabs& enemiesPrefabs, size_t currentWave, size_t waveOffSet, size_t waveStepOffSet, float minSpawnIntervale, float maxSpawnIntervale)
    :   ScriptComponent    {gameObject},
        _spawnerPrefabs                             {spawnerPrefabs},
        _enemiesPrefabs                             {enemiesPrefabs},
        _currentWave                                {currentWave},
        _waveOffSet                                 {waveOffSet},
        _waveStepOffSet                             {waveStepOffSet},
        _minSpawnIntervale                          {minSpawnIntervale},
        _maxSpawnIntervale                          {maxSpawnIntervale}
{
    GE_assert(maxSpawnIntervale >= minSpawnIntervale);
    GE_assert(minSpawnIntervale > 0.f);
    GE_assert(maxSpawnIntervale > 0.f);
    GE_assert(!spawnerPrefabs.empty());
    GE_assert(!enemiesPrefabs.empty());

    _name = __FUNCTION__;
}

WaveManager::WaveManager (GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent    {refGameObject},
        _spawnerPrefabs                             {},
        _enemiesPrefabs                             {},
        _currentWave                                {static_cast<size_t>(std::stoi(params[0]))},
        _minSpawnIntervale                          {std::stof(params[1])},
        _maxSpawnIntervale                          {std::stof(params[2])},
        _nextSpawnEnemie                            {std::stof(params[3])},
        _delay                                      {std::stof(params[4])},
        _numberEnemiesGenerate                      {static_cast<size_t>(std::stoi(params[5]))}
{
    _name = __FUNCTION__;

    size_t count = 6;

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

void WaveManager::start()
{

}

void WaveManager::update()
{

}

void WaveManager::nextWave()
{
    _currentWave++;

    /*Remove all spawner and all gameObject*/
    _gameObject.children.clear();

    /*Generate the number of ennemies in funciton of the current wave*/
    std::vector<unsigned int> contenorNumberEnemie; // from weakest to strongest
    int totalEnnemiesOnThisWave = generateNumberEnnemy (contenorNumberEnemie);
    
    /*Activate new spawner*/
    size_t numberSpawnActivate = totalEnnemiesOnThisWave / 5;
    if (numberSpawnActivate >_spawnerPrefabs.size())
    {
        numberSpawnActivate = _spawnerPrefabs.size();
    }
    
    /*Initialize the list of ennemies*/
    std::vector<EntityPrefabCount> enemiesList;

    for (size_t i = 0; i < _enemiesPrefabs.size(); i++)
    {
       enemiesList.push_back({contenorNumberEnemie[i], _enemiesPrefabs[i]});
    }

    /*Choose randomly the spawner activate*/
    SpawnerPrefabs copySpawnerPrefabs = _spawnerPrefabs;
    std::vector <CircularEntitiesSpawner*> spawnerList;

    for (size_t i = 0; i < numberSpawnActivate; i++)
    {
        int spawnerChoose = Engine::Core::Maths::Random::ranged<int>(copySpawnerPrefabs.size() - 1);
        GameObject& newSpawnerGO = Save::loadPrefab(_gameObject, copySpawnerPrefabs[spawnerChoose]);

        spawnerList.push_back(newSpawnerGO.getComponent<CircularEntitiesSpawner>());
        GE_assertInfo(spawnerList.back() != nullptr, "Spawner prefabs doesn't content any CircularEntitiesSpawner");

        copySpawnerPrefabs.erase(copySpawnerPrefabs.begin() + spawnerChoose);
    }

    /*Affect entities on the spawner 5 by 5*/
    while (!enemiesList.empty())
    {
        for (auto &&spawner : spawnerList)
        {
            for (size_t enemyTypeIndex = 0; enemyTypeIndex < enemiesList.size(); enemyTypeIndex++)
            {
                /*Insert entities 5 by 5 maximum (avoid unbalance spawner)*/
                unsigned int numberEntityInsert = Engine::Core::Maths::Random::ranged<int>(5);
                if (numberEntityInsert > enemiesList[enemyTypeIndex].numberEntity)
                {
                    numberEntityInsert = enemiesList[enemyTypeIndex].numberEntity;
                }

                spawner->addEntitiesToSpawner(numberEntityInsert, enemiesList[enemyTypeIndex].pathPrefabs);

                enemiesList[enemyTypeIndex].numberEntity -= numberEntityInsert;

                if (enemiesList[enemyTypeIndex].numberEntity == 0)
                {
                    enemiesList.erase(enemiesList.begin() + enemyTypeIndex);
                }
            }
        }
    }
}

void WaveManager::save(xml_document<>& doc, xml_node<>* nodeParent) 
{
    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    newNode->append_attribute(doc.allocate_attribute("currentWave", doc.allocate_string(std::to_string(_currentWave).c_str())));
    newNode->append_attribute(doc.allocate_attribute("minSpawnIntervale", doc.allocate_string(std::to_string(_minSpawnIntervale).c_str())));
    newNode->append_attribute(doc.allocate_attribute("maxSpawnIntervale", doc.allocate_string(std::to_string(_maxSpawnIntervale).c_str())));
    newNode->append_attribute(doc.allocate_attribute("nextSpawnEnemie", doc.allocate_string(std::to_string(_nextSpawnEnemie).c_str())));
    newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
    newNode->append_attribute(doc.allocate_attribute("numberEnemiesGenerate", doc.allocate_string(std::to_string(_numberEnemiesGenerate).c_str())));

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

int WaveManager::generateNumberEnnemy (std::vector<unsigned int>& refContenorNumberEnemie)
{
    const size_t numberTypeEnemie = _enemiesPrefabs.size();
    int enemiesCount = 0;
    int realCurrentWave = _waveOffSet + _currentWave * _waveStepOffSet;

    for (size_t i = 0; i < numberTypeEnemie; i++)
    {
        const int powEnemiesMinusOne = numberTypeEnemie * numberTypeEnemie - 1;
        const int enemies = (((realCurrentWave % powEnemiesMinusOne) & (int)std::pow(2, i)) >> i) + (realCurrentWave / powEnemiesMinusOne);
        refContenorNumberEnemie.push_back(enemies);
        enemiesCount += enemies;
    }

    return enemiesCount;
}
