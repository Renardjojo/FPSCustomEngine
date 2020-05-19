#include "GE/Ressources/Saves.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"

#include "GE/Ressources/Component.hpp"
#include "GE/Core/Component/ScriptComponent.hpp"
#include "Game/PlayerController.hpp"
#include "GE/Physics/PhysicalObject.hpp"
#include "GE/Physics/Collider.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"

using namespace rapidxml;

using namespace Game;
using namespace Engine;
using namespace Engine::Ressources;
using namespace Engine::Ressources::Save;
using namespace Engine::LowRenderer;
using namespace Engine::Core::Maths;
using namespace Engine::Physics;


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
            newGameObject->addComponent<Model>(newModel);

            
        for (std::string componentName : componentNames)
        {
            if (componentName.compare("PhysicalObject") == 0)
                newGameObject->addComponent<PhysicalObject>();

            else if (componentName.compare("SphereCollider") == 0)
                newGameObject->addComponent<SphereCollider>();

            else if (componentName.compare("OrientedBoxCollider") == 0)
                newGameObject->addComponent<OrientedBoxCollider>();
            
            else if (componentName.compare("PlayerController") == 0)
                newGameObject->addComponent<PlayerController>(gameEngine.inputManager_);
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
    else if (str.compare("DirLight") == 0)
    {
        Vec3 dir;
        Vec4 ambient;
        Vec4 diffuse;
        Vec4 specular;
        std::string name;
        std::vector<std::string> componentNames;

        for (; attr; attr = attr->next_attribute())
        {
            str = attr->name();

            if (str.compare("dirX") == 0)
                dir.x = std::stof(attr->value());

            else if (str.compare("dirY") == 0)
                dir.y = std::stof(attr->value());

            else if (str.compare("dirZ") == 0)
                dir.z = std::stof(attr->value());

            else if (str.compare("ambient0") == 0)
                ambient.x = std::stof(attr->value());

            else if (str.compare("ambient1") == 0)
                ambient.y = std::stof(attr->value());

            else if (str.compare("ambient2") == 0)
                ambient.z = std::stof(attr->value());

            else if (str.compare("ambient3") == 0)
                ambient.w = std::stof(attr->value());

            else if (str.compare("diffuse0") == 0)
                diffuse.x = std::stof(attr->value());

            else if (str.compare("diffuse1") == 0)
                diffuse.y = std::stof(attr->value());

            else if (str.compare("diffuse2") == 0)
                diffuse.z = std::stof(attr->value());

            else if (str.compare("diffuse3") == 0)
                diffuse.w = std::stof(attr->value());

            else if (str.compare("specular0") == 0)
                specular.x = std::stof(attr->value());

            else if (str.compare("specular1") == 0)
                specular.y = std::stof(attr->value());

            else if (str.compare("specular2") == 0)
                specular.z = std::stof(attr->value());

            else if (str.compare("specular3") == 0)
                specular.w = std::stof(attr->value());

            else if (str.compare("name") == 0)
                name = attr->value();

            else if (str.compare("componentName") == 0)
                componentNames.push_back(attr->value());

        }


        DirectionnalLightCreateArg dirLightArg  {dir,
                                                {ambient.x, ambient.y, ambient.z, ambient.w},
                                                {diffuse.x, diffuse.y, diffuse.z, diffuse.w},
                                                {specular.x, specular.y, specular.z, specular.w},
                                                name.c_str()};


        newGameObject = &scene.add<DirectionnalLight>(parent, dirLightArg);
        static_cast<DirectionnalLight*>(newGameObject->entity.get())->enable(true);
    }
    else if (str.compare("PointLight") == 0)
    {
        Vec3 pos;
        Vec4 ambient;
        Vec4 diffuse;
        Vec4 specular;
        Vec3 conslinQuad;
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

            else if (str.compare("ambient0") == 0)
                ambient.x = std::stof(attr->value());

            else if (str.compare("ambient1") == 0)
                ambient.y = std::stof(attr->value());

            else if (str.compare("ambient2") == 0)
                ambient.z = std::stof(attr->value());

            else if (str.compare("ambient3") == 0)
                ambient.w = std::stof(attr->value());

            else if (str.compare("diffuse0") == 0)
                diffuse.x = std::stof(attr->value());

            else if (str.compare("diffuse1") == 0)
                diffuse.y = std::stof(attr->value());

            else if (str.compare("diffuse2") == 0)
                diffuse.z = std::stof(attr->value());

            else if (str.compare("diffuse3") == 0)
                diffuse.w = std::stof(attr->value());

            else if (str.compare("specular0") == 0)
                specular.x = std::stof(attr->value());

            else if (str.compare("specular1") == 0)
                specular.y = std::stof(attr->value());

            else if (str.compare("specular2") == 0)
                specular.z = std::stof(attr->value());

            else if (str.compare("specular3") == 0)
                specular.w = std::stof(attr->value());
                
            else if (str.compare("constant") == 0)
                conslinQuad.x = std::stof(attr->value());

            else if (str.compare("linear") == 0)
                conslinQuad.y = std::stof(attr->value());

            else if (str.compare("quadriatic") == 0)
                conslinQuad.z = std::stof(attr->value());

            else if (str.compare("name") == 0)
                name = attr->value();

            else if (str.compare("componentName") == 0)
                componentNames.push_back(attr->value());

        }


        PointLightCreateArg PointLightArg  {pos,
                                                {ambient.x, ambient.y, ambient.z, ambient.w},
                                                {diffuse.x, diffuse.y, diffuse.z, diffuse.w},
                                                {specular.x, specular.y, specular.z, specular.w},
                                                conslinQuad.x, conslinQuad.y, conslinQuad.z,
                                                name.c_str()};


        newGameObject = &scene.add<PointLight>(parent, PointLightArg);
        static_cast<PointLight*>(newGameObject->entity.get())->enable(true);
    }

    for (xml_node<>* children = node->first_node(); children; children = children->next_sibling())
        initEntity(scene, *newGameObject, gameEngine, children);
}

void Engine::Ressources::Save::saveScene(Scene &scene, GE &gameEngine, const char *filePath)
{
    xml_document<> doc;
    xml_node<> *worldNode = doc.allocate_node(node_element, "WORLD");
    doc.append_node(worldNode);

    for (GameObject& gameObjectParent : scene.getWorld().children)
        saveEntity(gameObjectParent, doc, worldNode);

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

    if (dynamic_cast<Model*>(gameObjectParent.entity.get()))
    {
        Model* model = dynamic_cast<Model*>(gameObjectParent.entity.get());
        newNode->append_attribute(doc.allocate_attribute("type", "Model"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.entity->getName()));
        newNode->append_attribute(doc.allocate_attribute("posX", doc.allocate_string(std::to_string(gameObjectParent.entity->getPosition().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posY", doc.allocate_string(std::to_string(gameObjectParent.entity->getPosition().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("posZ", doc.allocate_string(std::to_string(gameObjectParent.entity->getPosition().z).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotX", doc.allocate_string(std::to_string(gameObjectParent.entity->getRotation().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotY", doc.allocate_string(std::to_string(gameObjectParent.entity->getRotation().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("rotZ", doc.allocate_string(std::to_string(gameObjectParent.entity->getRotation().z).c_str())));
        newNode->append_attribute(doc.allocate_attribute("scaleX", doc.allocate_string(std::to_string(gameObjectParent.entity->getScale().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("scaleY", doc.allocate_string(std::to_string(gameObjectParent.entity->getScale().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("scaleZ", doc.allocate_string(std::to_string(gameObjectParent.entity->getScale().z).c_str())));
        newNode->append_attribute(doc.allocate_attribute("shaderName", doc.allocate_string(model->getShaderName().c_str())));
        newNode->append_attribute(doc.allocate_attribute("materialName", doc.allocate_string(model->getMaterialName()[0].c_str())));
        newNode->append_attribute(doc.allocate_attribute("meshName", doc.allocate_string(model->getMeshName().c_str())));
        if (gameObjectParent.getComponent<PlayerController>())
            newNode->append_attribute(doc.allocate_attribute("componentName", "PlayerController"));
        if (gameObjectParent.getComponent<OrientedBoxCollider>())
            newNode->append_attribute(doc.allocate_attribute("componentName", "OrientedBoxCollider"));
        if (gameObjectParent.getComponent<SphereCollider>())
            newNode->append_attribute(doc.allocate_attribute("componentName", "SphereCollider"));
        if (gameObjectParent.getComponent<PhysicalObject>())
            newNode->append_attribute(doc.allocate_attribute("componentName", "PhysicalObject"));
    }
    else if (dynamic_cast<DirectionnalLight*>(gameObjectParent.entity.get()))
    {
        DirectionnalLight* dirLight = dynamic_cast<DirectionnalLight*>(gameObjectParent.entity.get());
        newNode->append_attribute(doc.allocate_attribute("type", "DirLight"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.entity->getName()));
        
        newNode->append_attribute(doc.allocate_attribute("dirX", doc.allocate_string(std::to_string(dirLight->getPosition().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("dirY", doc.allocate_string(std::to_string(dirLight->getPosition().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("dirZ", doc.allocate_string(std::to_string(dirLight->getPosition().z).c_str())));

        newNode->append_attribute(doc.allocate_attribute("ambient0", doc.allocate_string(std::to_string(dirLight->getAmbient().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient1", doc.allocate_string(std::to_string(dirLight->getAmbient().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient2", doc.allocate_string(std::to_string(dirLight->getAmbient().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient3", doc.allocate_string(std::to_string(dirLight->getAmbient().e[3]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse0", doc.allocate_string(std::to_string(dirLight->getDiffuse().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse1", doc.allocate_string(std::to_string(dirLight->getDiffuse().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse2", doc.allocate_string(std::to_string(dirLight->getDiffuse().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse3", doc.allocate_string(std::to_string(dirLight->getDiffuse().e[3]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular0", doc.allocate_string(std::to_string(dirLight->getSpecular().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular1", doc.allocate_string(std::to_string(dirLight->getSpecular().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular2", doc.allocate_string(std::to_string(dirLight->getSpecular().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular3", doc.allocate_string(std::to_string(dirLight->getSpecular().e[3]).c_str())));
    
    }
    else if (dynamic_cast<PointLight*>(gameObjectParent.entity.get()))
    {
        PointLight* ptLight = dynamic_cast<PointLight*>(gameObjectParent.entity.get());
        newNode->append_attribute(doc.allocate_attribute("type", "PointLight"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.entity->getName()));
        
        newNode->append_attribute(doc.allocate_attribute("dirX", doc.allocate_string(std::to_string(ptLight->getPosition().x).c_str())));
        newNode->append_attribute(doc.allocate_attribute("dirY", doc.allocate_string(std::to_string(ptLight->getPosition().y).c_str())));
        newNode->append_attribute(doc.allocate_attribute("dirZ", doc.allocate_string(std::to_string(ptLight->getPosition().z).c_str())));

        newNode->append_attribute(doc.allocate_attribute("ambient0", doc.allocate_string(std::to_string(ptLight->getAmbient().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient1", doc.allocate_string(std::to_string(ptLight->getAmbient().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient2", doc.allocate_string(std::to_string(ptLight->getAmbient().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("ambient3", doc.allocate_string(std::to_string(ptLight->getAmbient().e[3]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse0", doc.allocate_string(std::to_string(ptLight->getDiffuse().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse1", doc.allocate_string(std::to_string(ptLight->getDiffuse().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse2", doc.allocate_string(std::to_string(ptLight->getDiffuse().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("diffuse3", doc.allocate_string(std::to_string(ptLight->getDiffuse().e[3]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular0", doc.allocate_string(std::to_string(ptLight->getSpecular().e[0]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular1", doc.allocate_string(std::to_string(ptLight->getSpecular().e[1]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular2", doc.allocate_string(std::to_string(ptLight->getSpecular().e[2]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("specular3", doc.allocate_string(std::to_string(ptLight->getSpecular().e[3]).c_str())));
        newNode->append_attribute(doc.allocate_attribute("constant", doc.allocate_string(std::to_string(ptLight->getConstant()).c_str())));
        newNode->append_attribute(doc.allocate_attribute("linear", doc.allocate_string(std::to_string(ptLight->getLinear()).c_str())));
        newNode->append_attribute(doc.allocate_attribute("quadriatic", doc.allocate_string(std::to_string(ptLight->getQuadratic()).c_str())));
    
    }
    else if (dynamic_cast<Camera*>(gameObjectParent.entity.get()))
    {
        Camera* camera = dynamic_cast<Camera*>(gameObjectParent.entity.get());

        newNode->append_attribute(doc.allocate_attribute("type", "Camera"));
        newNode->append_attribute(doc.allocate_attribute("name", gameObjectParent.entity->getName()));
        
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
    else // TODO: Assert
    {
        std::cout << "entity not define in save" << std::endl;
        return;
    }

    for (GameObject &gameObjectParent : gameObjectParent.children)
        saveEntity(gameObjectParent, doc, newNode);

    nodeParent->append_node(newNode);
}