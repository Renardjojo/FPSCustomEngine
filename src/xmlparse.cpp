#include "save/xmlparse.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Ressources/Component.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Component/ScriptComponent.hpp"
#include "Game/PlayerController.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/Physics/ColliderShape/CapsuleCollider.hpp"
#include "GE/Physics/ColliderShape/AABBCollider.hpp"
#include "GE/LowRenderer/camera.hpp"

using namespace rapidxml;
using namespace Engine::Ressources;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Game;

void setupScene(Scene& scene, Engine::GE& gameEngine, const char* filePath)
{
    file<> xmlFile(filePath);
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    xml_node<>* node = doc.first_node();

    for (xml_node<>* children = node->first_node(); children; children = children->next_sibling())
                initEntity(scene, scene.getWorld(), gameEngine, children);
}

void initEntity(Scene& scene, Engine::Ressources::GameObject& parent, Engine::GE& gameEngine, xml_node<>* node)
{
    GameObject* newGameObject = nullptr;

    xml_attribute<>* attr = node->first_attribute();

    std::string str = std::string(attr->value());

    if (str.compare("Model") == 0)
    {
        Vec3 pos, rot, scale;
        std::string name, shaderName, materialName, meshName;
        std::vector<std::string> componentNames;

        for (; attr; attr = attr->next_attribute())
        {
            str = attr->name();

            if (str.compare("posX") == 0)
                pos.x = std::stof(attr->value());

            else if (str.compare("posY") == 0)
                pos.y = std::stof(attr->value());

            else if (str.compare("posZ") == 0)
                pos.z = std::stof(attr->value());

            else if (str.compare("rotX") == 0)
                rot.x = std::stof(attr->value());

            else if (str.compare("rotY") == 0)
                rot.y = std::stof(attr->value());

            else if (str.compare("rotZ") == 0)
                rot.z = std::stof(attr->value());

            else if (str.compare("scaleX") == 0)
                scale.x = std::stof(attr->value());

            else if (str.compare("scaleY") == 0)
                scale.y = std::stof(attr->value());

            else if (str.compare("scaleZ") == 0)
                scale.z = std::stof(attr->value());

            else if (str.compare("name") == 0)
                name = attr->value();

            else if (str.compare("shaderName") == 0)
                shaderName = attr->value();

            else if (str.compare("materialName") == 0)
                materialName = attr->value();

            else if (str.compare("meshName") == 0)
                meshName = attr->value();

            else if (str.compare("componentName") == 0)
                componentNames.push_back(attr->value());
        }

        GameObjectCreateArg newGameOBject =   {name.c_str(), pos, rot, scale};

        ModelCreateArg newModel =   {&gameEngine.ressourceManager_.get<Shader>(shaderName), 
                                    {&gameEngine.ressourceManager_.get<Material>(materialName)}, 
                                    &gameEngine.ressourceManager_.get<Mesh>(meshName)};

        newGameObject = &scene.add<GameObject>(parent, newGameOBject);

        if (newGameObject != nullptr)
        {
            newGameObject->addComponent<Model>(newModel);
        }

        for (std::string componentName : componentNames)
        {
            if (componentName.compare("PhysicalObject") == 0)
                newGameObject->addComponent<PhysicalObject>();

            else if (componentName.compare("SphereCollider") == 0)
                newGameObject->addComponent<SphereCollider>();

            else if (componentName.compare("OrientedBoxCollider") == 0)
                newGameObject->addComponent<OrientedBoxCollider>();
            
            else if (componentName.compare("PlayerController") == 0)
                newGameObject->addComponent<PlayerController>();
        }
    }
    else if (str.compare("Camera") == 0)
    {
        Vec3 pos, rot;
        float near = 0.0f;
        float far = 0.0f;
        float fov = 0.0f;
        std::string name;
        std::vector<std::string> componentNames;

        for (; attr; attr = attr->next_attribute())
        {
            str = attr->name();

            if (str.compare("posX") == 0)
                pos.x = std::stof(attr->value());

            else if (str.compare("posY") == 0)
                pos.y = std::stof(attr->value());

            else if (str.compare("posZ") == 0)
                pos.z = std::stof(attr->value());

            else if (str.compare("rotX") == 0)
                rot.x = std::stof(attr->value());

            else if (str.compare("rotY") == 0)
                rot.y = std::stof(attr->value());

            else if (str.compare("rotZ") == 0)
                rot.z = std::stof(attr->value());

            else if (str.compare("near") == 0)
                near = std::stof(attr->value());

            else if (str.compare("far") == 0)
                far = std::stof(attr->value());

            else if (str.compare("fov") == 0)
                fov = std::stof(attr->value());

            else if (str.compare("name") == 0)
                name = attr->value();

            else if (str.compare("componentName") == 0)
                componentNames.push_back(attr->value());
        }

        CameraPerspectiveCreateArg camArg {pos, rot, gameEngine.getWinSize().width / static_cast<float>(gameEngine.getWinSize().heigth), near, far, fov, name.c_str()};
        newGameObject = &scene.add<Camera>(scene.getWorld(), camArg);
        dynamic_cast<Camera *>(newGameObject)->use();
    }
    else if (str.compare("Entity") == 0)
    {
        // an other for
    }

    for (xml_node<>* children = node->first_node(); children; children = children->next_sibling())
        initEntity(scene, *newGameObject, gameEngine, children);
}