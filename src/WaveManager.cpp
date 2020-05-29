#include "Game/WaveManager.hpp"

#include "GE/Ressources/scene.hpp"
#include "GE/Ressources/Saves.hpp"
#include "GE/Core/Maths/Random.hpp"

#include "Game/CircularEntitiesSpawner.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Component;
using namespace Engine::Core::Maths;

WaveManager::WaveManager(GameObject &gameObject, const SpawnerPrefabs& spawnerPrefabs, const EnemiesPrefabs& enemiesPrefabs, size_t currentWave, size_t waveOffSet, size_t waveStepOffSet, float timeBeforeNextWave)
    :   ScriptComponent                             {gameObject},
        _spawnerPrefabs                             {spawnerPrefabs},
        _enemiesPrefabs                             {enemiesPrefabs},
        _currentWave                                {currentWave},
        _waveOffSet                                 {waveOffSet},
        _waveStepOffSet                             {waveStepOffSet},
        _timeBeforeNextWave                         {timeBeforeNextWave},
        _delay                                      {0.f}
{
    GE_assert(waveStepOffSet != 0.f);
    GE_assert(!spawnerPrefabs.empty());
    GE_assert(!enemiesPrefabs.empty());

    _name = __FUNCTION__;
}

WaveManager::WaveManager (GameObject &refGameObject, const std::vector<std::string>& params)
    :   ScriptComponent                             {refGameObject},
        _spawnerPrefabs                             {},
        _enemiesPrefabs                             {},
        _currentWave                                {static_cast<size_t>(std::stoi(params[0]))},
        _waveOffSet                                 {static_cast<size_t>(std::stoi(params[1]))},
        _waveStepOffSet                             {static_cast<size_t>(std::stoi(params[2]))},
        _timeBeforeNextWave                         {std::stof(params[3])},
        _delay                                      {std::stof(params[4])}
{
    _name = __FUNCTION__;

    size_t count = 5;

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
    _enemiesContenor = &Scene::getCurrentScene()->getGameObject("EnemiesContener");
    GE_assertInfo(_enemiesContenor != nullptr, "Game object name \"EnemiesContener\" does'nt exist on world");
}

void WaveManager::update()
{
    /*Go the next wave if all entity is dead and all spawner is empty*/
    if (_gameObject.children.empty() && _enemiesContenor->children.empty())
    {
        _delay += Engine::Core::System::TimeSystem::getDeltaTime();

        if(_delay >= _timeBeforeNextWave)
        {
            _delay = 0.f;
            nextWave();
        }
        return;
    }

    /*Check and remove the the empty spawner*/
    for (std::list<std::unique_ptr<GameObject>>::iterator it =  _gameObject.children.begin(); it != _gameObject.children.end(); it++)
    {
        CircularEntitiesSpawner* pSpawner = (*it)->getComponent<CircularEntitiesSpawner>();
        GE_assertInfo(pSpawner != nullptr, "WaveManager contain gameObject that is not a spawner. WaveManager must be empty");
    
        if (pSpawner->isEmpty())
        {
            it = _gameObject.destroyChild(it);
        }
    }
}

void WaveManager::nextWave()
{
    _currentWave ++;

    /*Remove all spawner and all gameObject*/
    _gameObject.children.clear();

    /*Generate the number of ennemies in funciton of the current wave*/
    std::vector<unsigned int> contenorNumberEnemie; // from weakest to strongest
    int totalEnnemiesOnThisWave = generateNumberEnnemy (contenorNumberEnemie);

    GE_assertInfo(totalEnnemiesOnThisWave != 0, "Anought ennemies for with wave");

    /*Activate new spawner*/
    size_t numberSpawnActivate = totalEnnemiesOnThisWave / 5 + 1;
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
    newNode->append_attribute(doc.allocate_attribute("waveOffSet", doc.allocate_string(std::to_string(_waveOffSet).c_str())));
    newNode->append_attribute(doc.allocate_attribute("waveStepOffSet", doc.allocate_string(std::to_string(_waveStepOffSet).c_str())));
    newNode->append_attribute(doc.allocate_attribute("timeBeforeNextWave", doc.allocate_string(std::to_string(_timeBeforeNextWave).c_str())));
    newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));

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
    size_t numberTypeEnemie = _enemiesPrefabs.size();
    int enemiesCount = 0;
    int realCurrentWave = _waveOffSet + _currentWave * _waveStepOffSet;

    for (size_t i = 0; i < numberTypeEnemie; i++)
    {
        int powEnemiesMinusOne = numberTypeEnemie * numberTypeEnemie - 1;
        unsigned int enemies = (((realCurrentWave % powEnemiesMinusOne) & (int)std::pow(2, i)) >> i) + (realCurrentWave / powEnemiesMinusOne);
        refContenorNumberEnemie.push_back(enemies);
        enemiesCount += enemies;
    }

    return enemiesCount;
}
