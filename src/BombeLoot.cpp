#include "Game/BombeLoot.hpp"

using namespace Game;

BombeLoot::BombeLoot(Engine::Ressources::GameObject &gameObject)
    :   Loot    {gameObject}
{
    _name = __FUNCTION__;
}

BombeLoot::BombeLoot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   Loot    {refGameObject, params}
{
    _name = __FUNCTION__;
}

void BombeLoot::effect()
{
    /*Destroy all ennemies on map but not inside the spawner*/
    Engine::Ressources::Scene::getCurrentScene()->getGameObject("EnemiesContener").children.clear();

    Engine::LowRenderer::ModelCreateArg modelArg3{&Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Shader>("ColorWithLight"),
                            &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Engine::Ressources::Material>>("YellowMaterial"),
                            &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Mesh>("PlaneZ"),
                            "ColorWithLight",
                            "YellowMaterial",
                            "PlaneZ"};

    ParticuleGenerator::ParticleSystemCreateArg particalArg;
    particalArg.modelCreateArg = modelArg3;
    particalArg.isBillBoard = false;
    particalArg.modelCreateArg.enableBackFaceCulling = false;
    particalArg.physicalObjectCreateArg.useGravity = false;
    particalArg.velocityEvolutionCoef = 0.8f;
    particalArg.propulsionLenght = 10.f;
    particalArg.spawnCountBySec = 35.f;
    particalArg.lifeDuration = 0.f;
    particalArg.scale = {0.1, 0.2f, 0.1};
    particalArg.generationRange = 3.f;
    particalArg.instanteParticuleGeneration = true;

    Engine::Ressources::GameObject& particleGO = Engine::Ressources::Scene::getCurrentScene()->add<Engine::Ressources::GameObject>(Engine::Ressources::Scene::getCurrentScene()->getWorld(), Engine::Ressources::GameObjectCreateArg{"ParticleContenerBombeLootEffect", {_playerGO->getGlobalPosition()}});
    particleGO.addComponent<ParticuleGenerator>(particalArg);
    particleGO.addComponent<LifeDuration>(1.5f);
}