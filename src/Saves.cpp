#include "GE/Ressources/Saves.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"

#include "GE/Ressources/Component.hpp"
#include "GE/Core/Component/ScriptComponent.hpp"
#include "Game/PlayerController.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/ColliderShape/Collider.hpp"
#include "GE/Physics/ColliderShape/SphereCollider.hpp"
#include "GE/Physics/ColliderShape/OrientedBoxCollider.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include "GE/Core/System/ScriptSystem.hpp"

using namespace rapidxml;

using namespace Game;
using namespace Engine;
using namespace Engine::Ressources;
using namespace Engine::Ressources::Save;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Physics;
using namespace Engine::Physics::ColliderShape;
using namespace Engine::Core::System;


void Engine::Ressources::Save::initSavePaths(std::vector<std::string> &savePaths, const char *path)
{
    std::string head;

    std::ifstream file(path);

    if (!file.is_open())
    {
        std::cout << "FAIL TO READ SAVE FILE" << std::endl; // TODO: assert
        return;
    }

    while (file.good())
    {
        file >> head;

        savePaths.push_back(head);
    }

    file.close();
}

void Engine::Ressources::Save::addSavePath(std::vector<std::string> &savePaths, const char *path)
{
    std::string head;
    std::string pathStr = path;

    std::ifstream file("./ressources/saves/LevelPaths");
    std::ofstream out;
    out.open("./ressources/saves/LevelPaths", std::ios::app);
    
    if (!file.is_open() || !out.is_open())
    {
        std::cout << "FAIL TO READ SAVE FILE" << std::endl; // TODO: assert
        return;
    }

    while (file.good())
    {
        file >> head;

        if (pathStr.compare(head) == 0)
        {
            file.close();
            return;
        }
    }

    out << "\n" << pathStr;
    savePaths.push_back(path);

    out.close();
    file.close();
}

void Engine::Ressources::Save::setupScene(Scene& scene, Engine::GE& gameEngine, const char* filePath)
{
    file<> xmlFile(filePath);
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    xml_node<>* node = doc.first_node();

    for (xml_node<>* children = node->first_node(); children; children = children->next_sibling())
        initEntity(scene, scene.getWorld(), gameEngine, children);

    ScriptSystem::start();
}

void Engine::Ressources::Save::loadPrefab(Scene& scene, GameObject& parent, Engine::GE& gameEngine, const char* filePath)
{
    file<> xmlFile(filePath);
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    xml_node<>* node = doc.first_node();

    initEntity(scene, parent, gameEngine, node);
}

void Engine::Ressources::Save::initEntity(Scene& scene, Engine::Ressources::GameObject& parent, Engine::GE& gameEngine, xml_node<>* node)
{
    GameObject* newGameObject = nullptr;

    xml_attribute<>* attr = node->first_attribute();

    std::string str = std::string(attr->value());

    if (std::string(node->name()).compare("GAMEOBJECT") == 0) // Gameobject
    {
        if (str.compare("Empty") == 0)
        {
            Vec3 pos, rot, scale;
            std::string name;

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
            }

            GameObjectCreateArg gameObjectArg{name, {pos, rot, scale}};
            newGameObject = &scene.add<GameObject>(parent, gameObjectArg);
        }
        else if (str.compare("Camera") == 0)
        {
            Vec3 pos, rot;
            float near = 0.0f;
            float far = 0.0f;
            float fov = 0.0f;
            std::string name;

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
            }

            CameraPerspectiveCreateArg camArg{pos, rot, gameEngine.getWinSize().width / static_cast<float>(gameEngine.getWinSize().heigth), near, far, fov, name.c_str()};
            newGameObject = &scene.add<Camera>(scene.getWorld(), camArg);
            dynamic_cast<Camera *>(newGameObject)->use();
        }
    } // Gameobject
    else if (std::string(node->name()).compare("COMPONENT") == 0)// Component
    {
        std::string type = str;
        std::vector<std::unique_ptr<std::string>> params;
        attr = attr->next_attribute();
        for (; attr; attr = attr->next_attribute())
            params.push_back(std::make_unique<std::string>(attr->value()));

        if (type.compare("Model") == 0)
            parent.addComponent<Model>(params, gameEngine.ressourceManager_);
        else if (type.compare("PhysicalObject") == 0)
            parent.addComponent<PhysicalObject>();
        else if (type.compare("OrientedBoxCollider") == 0)
            parent.addComponent<OrientedBoxCollider>();
        else if (type.compare("SphereCollider") == 0)
            parent.addComponent<SphereCollider>();
        else if (type.compare("PlayerController") == 0)
            parent.addComponent<PlayerController>();
        else if (type.compare("DirectionnalLight") == 0)
        {
            parent.addComponent<DirectionnalLight>(params);
            parent.getComponent<DirectionnalLight>()->enable(true);
        }
        else if (type.compare("PointLight") == 0)
        {
            parent.addComponent<PointLight>(params);
            parent.getComponent<PointLight>()->enable(true);
        }



        newGameObject = &parent;
    }

    for (xml_node<>* children = node->first_node(); children; children = children->next_sibling())
        initEntity(scene, *newGameObject, gameEngine, children);
}

void Engine::Ressources::Save::saveScene(Scene &scene, GE &gameEngine, const char *filePath)
{
    xml_document<> doc;
    xml_node<> *worldNode = doc.allocate_node(node_element, "WORLD");
    doc.append_node(worldNode);

    for (auto&& gameObjectParent : scene.getWorld().children)
        saveEntity(*gameObjectParent, doc, worldNode);

    std::ofstream file_stored(filePath);
    file_stored << doc;
    file_stored.close();
    doc.clear();

    addSavePath(gameEngine.savePaths, filePath);
}

void Engine::Ressources::Save::createPrefab(GameObject& gameObject, std::string prefabName)
{
    xml_document<> doc;
    xml_node<> *prefabNode = doc.allocate_node(node_element, prefabName.c_str());
    doc.append_node(prefabNode);

    saveEntity(gameObject, doc, prefabNode);

    std::ofstream file_stored("./ressources/Prefabs/" + prefabName);
    file_stored << doc;
    file_stored.close();
    doc.clear();

    // TODO: add prefab path
}

void Engine::Ressources::Save::saveEntity(GameObject& gameObjectParent, xml_document<>& doc, xml_node<>* nodeParent)
{
    xml_node<> *newNode = doc.allocate_node(node_element, "GAMEOBJECT");

    if (dynamic_cast<Camera*>(&gameObjectParent))
    {
        Camera* camera = dynamic_cast<Camera*>(&gameObjectParent);

        newNode->append_attribute(doc.allocate_attribute("type", "Camera"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.getCName()));
        
        newNode->append_attribute(doc.allocate_attribute("posX", doc.allocate_string(std::to_string(camera->getPosition().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posY", doc.allocate_string(std::to_string(camera->getPosition().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posZ", doc.allocate_string(std::to_string(camera->getPosition().z).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotX", doc.allocate_string(std::to_string(camera->getRotation().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotY", doc.allocate_string(std::to_string(camera->getRotation().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotZ", doc.allocate_string(std::to_string(camera->getRotation().z).c_str())));
        newNode->append_attribute(doc.allocate_attribute("near", doc.allocate_string(std::to_string(camera->getProjectionInfo().near).c_str())));
        newNode->append_attribute(doc.allocate_attribute("far", doc.allocate_string(std::to_string(camera->getProjectionInfo().far).c_str())));
        newNode->append_attribute(doc.allocate_attribute("fov", doc.allocate_string(std::to_string(camera->getProjectionInfo().fovX).c_str())));
    }
    else
    {
        newNode->append_attribute(doc.allocate_attribute("type", "Empty"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.getCName()));
        
        newNode->append_attribute(doc.allocate_attribute("posX", doc.allocate_string(std::to_string(gameObjectParent.getPosition().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posY", doc.allocate_string(std::to_string(gameObjectParent.getPosition().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posZ", doc.allocate_string(std::to_string(gameObjectParent.getPosition().z).c_str())));

        newNode->append_attribute(doc.allocate_attribute("rotX", doc.allocate_string(std::to_string(gameObjectParent.getRotation().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotY", doc.allocate_string(std::to_string(gameObjectParent.getRotation().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotZ", doc.allocate_string(std::to_string(gameObjectParent.getRotation().z).c_str())));

        newNode->append_attribute(doc.allocate_attribute("scaleX", doc.allocate_string(std::to_string(gameObjectParent.getScale().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("scaleY", doc.allocate_string(std::to_string(gameObjectParent.getScale().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("scaleZ", doc.allocate_string(std::to_string(gameObjectParent.getScale().z).c_str())));

        std::cout << "saving : " + gameObjectParent.getName() << std::endl;
    }

    if (gameObjectParent.getComponent<Model>())
        gameObjectParent.getComponent<Model>()->save(doc, newNode);

    if (gameObjectParent.getComponent<PlayerController>())
        gameObjectParent.getComponent<PlayerController>()->save(doc, newNode);

    if (gameObjectParent.getComponent<OrientedBoxCollider>())
        gameObjectParent.getComponent<OrientedBoxCollider>()->save(doc, newNode);

    if (gameObjectParent.getComponent<SphereCollider>())
        gameObjectParent.getComponent<SphereCollider>()->save(doc, newNode);

    if (gameObjectParent.getComponent<PhysicalObject>())
        gameObjectParent.getComponent<PhysicalObject>()->save(doc, newNode);

    if (gameObjectParent.getComponent<PointLight>())
        gameObjectParent.getComponent<PointLight>()->save(doc, newNode);

    if (gameObjectParent.getComponent<DirectionnalLight>())
        gameObjectParent.getComponent<DirectionnalLight>()->save(doc, newNode);

        

    for (auto&& gameObjectParent : gameObjectParent.children)
        saveEntity(*gameObjectParent, doc, newNode);

    nodeParent->append_node(newNode);
}