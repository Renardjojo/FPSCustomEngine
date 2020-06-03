#include "Game/LifeLoot.hpp"

#include <vector>

using namespace Game;

LifeLoot::LifeLoot(Engine::Ressources::GameObject &gameObject)
    :   Loot    {gameObject}
{
    _name = __FUNCTION__;
}

LifeLoot::LifeLoot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   Loot    {refGameObject, params}
{
    _name = __FUNCTION__;
}

void LifeLoot::effect()
{
    /*Give 2 health*/
    _playerGO->getComponent<PlayerController>()->inflictDamage(-2);

    Engine::LowRenderer::ModelCreateArg modelArg3{&Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Shader>("ColorWithLight"),
                            &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Engine::Ressources::Material>>("RedMaterial"),
                            &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Mesh>("PlaneZ"),
                            "ColorWithLight",
                            "RedMaterial",
                            "PlaneZ"};

    ParticuleGenerator::ParticleSystemCreateArg particalArg;
    particalArg.modelCreateArg = modelArg3;
    particalArg.isBillBoard = false;
    particalArg.modelCreateArg.enableBackFaceCulling = false;
    particalArg.physicalObjectCreateArg.useGravity = false;
    particalArg.velocityEvolutionCoef = 0.8f;
    particalArg.propulsionLenght = 5.f;
    particalArg.spawnCountBySec = 35.f;
    particalArg.lifeDuration = 0.f;
    particalArg.scale = {0.1, 0.2f, 0.1};
    particalArg.generationRange = 3.f;
    particalArg.instanteParticuleGeneration = true;

    Engine::Ressources::GameObject& particleGO = Engine::Ressources::Scene::getCurrentScene()->add<Engine::Ressources::GameObject>(Engine::Ressources::Scene::getCurrentScene()->getWorld(), Engine::Ressources::GameObjectCreateArg{"ParticleContenerLifeLootEffect", {_playerGO->getGlobalPosition()}});
    particleGO.addComponent<ParticuleGenerator>(particalArg);
    particleGO.addComponent<LifeDuration>(1.f);
}