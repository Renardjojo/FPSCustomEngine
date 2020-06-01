//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-22 - 17 h 39

#ifndef _GROUND_CONTROLLER_H
#define _GROUND_CONTROLLER_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/scene.hpp"

#include "Game/ParticuleGenerator.hpp"
#include "Game/LifeDuration.hpp"

namespace Game
{
    class GroundController : public Engine::Core::Component::ScriptComponent
    {
        private:


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
        GroundController(Engine::Ressources::GameObject &gameObject)
            :   Engine::Core::Component::ScriptComponent    {gameObject}
        {
            _name = __FUNCTION__;
        }

        virtual ~GroundController() = default;

        void onCollisionEnter (Engine::Physics::ColliderShape::HitInfo& hitInfo) override
        {
            if (hitInfo.gameObject->getTag() == "Bullet")
            {
                Engine::LowRenderer::ModelCreateArg modelArg3{&Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Shader>("Color"),
                                        &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Engine::Ressources::Material>>("BrownMaterial"),
                                        &Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Mesh>("PlaneZ"),
                                        "Color",
                                        "BrownMaterial",
                                        "PlaneZ"};

                ParticuleGenerator::ParticleSystemCreateArg particalArg;
                particalArg.modelCreateArg = modelArg3;
                particalArg.isBillBoard = false;
                particalArg.modelCreateArg.enableBackFaceCulling = false;
                particalArg.physicalObjectCreateArg.useGravity = true;
                particalArg.velocityEvolutionCoef = 0.8f;
                particalArg.propulsionLenght = 13.f;
                particalArg.spawnCountBySec = 25.f;
                particalArg.lifeDuration = 0.f;
                particalArg.scale = {0.1, 0.1f, 0.1};
                particalArg.generationRange = 0.1f;
                particalArg.instanteParticuleGeneration = true;


                Engine::Ressources::GameObject& particleGO = Engine::Ressources::Scene::getCurrentScene()->add<Engine::Ressources::GameObject>(Engine::Ressources::Scene::getCurrentScene()->getWorld(), Engine::Ressources::GameObjectCreateArg{"ParticleContenerBlood", {hitInfo.intersectionsInfo.intersection1}});
                particleGO.addComponent<ParticuleGenerator>(particalArg);
                particleGO.addComponent<LifeDuration>(1.f);
            }
        }

        void save(xml_document<>& doc, xml_node<>* nodeParent)
        {
            if (!nodeParent)
                return;

            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");
            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

            nodeParent->append_node(newNode);   
        }
    };

} //namespace Game

#endif //_GROUND_CONTROLLER_H