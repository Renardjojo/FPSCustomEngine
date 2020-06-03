//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _BOMBE_LOOT_H
#define _BOMBE_LOOT_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Random.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/shader.hpp"

#include "Game/ParticuleGenerator.hpp"
#include "Game/Loot.hpp"
#include "Game/PlayerController.hpp"
#include "Game/LifeDuration.hpp"

#include <vector>

namespace Game
{
    class BombeLoot : public Loot
    {
        protected :

        public:

        BombeLoot(Engine::Ressources::GameObject &gameObject)
            :   Loot    {gameObject}
        {
            _name = __FUNCTION__;
        }

        BombeLoot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Loot    {refGameObject, params}
        {
            _name = __FUNCTION__;
        }

        virtual ~BombeLoot() = default;


        virtual void effect()
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
    };

} //namespace Game

#endif //_BOMBE_LOOT_H