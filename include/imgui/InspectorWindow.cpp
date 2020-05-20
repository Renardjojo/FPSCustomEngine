#include "GE/LowRenderer/EditorTools/InspectorWindow.hpp"
#include <typeinfo> 

using namespace Engine::Ressources;
using namespace Engine::LowRenderer::EditorTools;
        
void InspectorWindow::updateTransform(GameObject& gameObject)
{
    if (!ImGui::CollapsingHeader("Transform"))
        return;

    Engine::Core::Maths::Vec3 pos = gameObject.getPosition();
    Engine::Core::Maths::Vec3 rot = gameObject.getRotation();
    Engine::Core::Maths::Vec3 scale = gameObject.getScale();

    ImGui::Columns(4);
    ImGui::Text("Position :");
    ImGui::NextColumn();
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##posX", &pos.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##posY", &pos.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##posZ", &pos.z, 0.1f);
    ImGui::NextColumn();

    ImGui::Text("Rotation :");
    ImGui::NextColumn();
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##rotX", &rot.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##rotY", &rot.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##rotZ", &rot.z, 0.1f);
    ImGui::NextColumn();

    ImGui::Text("Scale :");
    ImGui::NextColumn();
    ImGui::Text("X :"); ImGui::SameLine(); ImGui::DragFloat("##scaleX", &scale.x, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Y :"); ImGui::SameLine(); ImGui::DragFloat("##scaleY", &scale.y, 0.1f);
    ImGui::NextColumn();
    ImGui::Text("Z :"); ImGui::SameLine(); ImGui::DragFloat("##scaleZ", &scale.z, 0.1f);
    ImGui::Columns(1);

    gameObject.setTranslation(pos);
    gameObject.setRotation(rot);
    gameObject.setScale(scale);
}

void InspectorWindow::updateHeader(GameObject& gameObject)
{
    char gameObjectName[128]; //Max name lenght

    for (int i = 0; i < sizeof(gameObjectName); i++)
    { 
        gameObjectName[i] = gameObject.getName()[i]; 
    } 

    ImGui::Text("Name :"); ImGui::SameLine();
    if (ImGui::InputText("##name", gameObjectName, IM_ARRAYSIZE(gameObjectName)))
    {
        std::string newName = gameObjectName;
        if (!newName.empty())
        {
            gameObject.setName(newName.c_str());
        }
    }
}

void InspectorWindow::updateComponent(Engine::Ressources::GameObject& gameObject)
{
    for (auto &&i : gameObject.getComponents())
    {
        std::string compName = typeid(*i).name();
        compName = compName.substr(compName.find_last_of('4')+1);
        ImGui::CollapsingHeader(compName.c_str());
    }
}

        
bool InspectorWindow::isCloseAfterUpdate(GameObject& gameObject)
{
    bool open = true;
    ImGui::Begin("Inspector", &open);

    updateHeader    (gameObject);
    updateTransform (gameObject);
    updateComponent (gameObject);

    ImGui::End();

    return !open;
}