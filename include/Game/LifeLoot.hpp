//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _LIFE_LOOT_H
#define _LIFE_LOOT_H

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
    class LifeLoot : public Loot
    {
        protected :

        public:

        LifeLoot(Engine::Ressources::GameObject &gameObject)
            :   Loot    {gameObject}
        {
            _name = __FUNCTION__;
        }

        LifeLoot (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Loot    {refGameObject, params}
        {
            _name = __FUNCTION__;
        }

        virtual ~LifeLoot() = default;


        virtual void effect()
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
    };

} //namespace Game

#endif //_LIFE_LOOT_H