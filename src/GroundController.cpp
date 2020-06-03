#include "Game/GroundController.hpp"

#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/material.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/scene.hpp"
#include "GE/LowRenderer/ParticleSystemFactory.hpp"
#include "GE/Core/Maths/vec.hpp"

#include "Game/ParticuleGenerator.hpp"
#include "Game/LifeDuration.hpp"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Core::Component;
using namespace Engine::Core::Maths;

GroundController::GroundController(GameObject &gameObject)
    :   ScriptComponent    {gameObject}
{
    _name = __FUNCTION__;
}

void GroundController::onCollisionEnter (HitInfo& hitInfo)
{
    if (hitInfo.gameObject->getTag() == "Bullet")
    {
        ModelCreateArg modelArg3{&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("Color"),
                                &t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>("BrownMaterial"),
                                &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("PlaneZ"),
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

        GameObject& particleGO = Scene::getCurrentScene()->add<GameObject>(Scene::getCurrentScene()->getWorld(), GameObjectCreateArg{"ParticleContenerBlood", {hitInfo.intersectionsInfo.intersection1}});
        particleGO.addComponent<ParticuleGenerator>(particalArg);
        particleGO.addComponent<LifeDuration>(1.f);

        GameObjectCreateArg decaleGOPref {"bulletHoleDecal", hitInfo.intersectionsInfo.intersection1};
        decaleGOPref.transformArg.scale = Vec3::one * 0.4f;
        ModelCreateArg      modelDecaleGOPref   {&t_RessourcesManager::getRessourceManagerUse()->get<Shader>("LightAndTexture"),
                                                &t_RessourcesManager::getRessourceManagerUse()->get<std::vector<Material>>("BulletHole"),
                                                &t_RessourcesManager::getRessourceManagerUse()->get<Mesh>("Plane"),
                                                "LightAndTexture",
                                                "BulletHole",
                                                "Plane"};

        ParticleSystemFactory::createDecale(Scene::getCurrentScene()->getGameObject("world/DecalContenor"), decaleGOPref, modelDecaleGOPref, hitInfo.intersectionsInfo.normalI1);
    }
}

void GroundController::save(xml_document<>& doc, xml_node<>* nodeParent)
{
    if (!nodeParent)
        return;

    xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");
    newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));

    nodeParent->append_node(newNode);   
}