#include "GE/LowRenderer/EditorTools/InspectorWindow.hpp"

using namespace Engine::Ressources;
using namespace Engine::LowRenderer::EditorTools;
        
bool InspectorWindow::isCloseAfterUpdate(Engine::Ressources::GameObject& gameObject)
{
    bool open = true;
    ImGui::Begin("Inspector", &open);

    char gameObjectName[128]; //Max name lenght

    for (int i = 0; i < sizeof(gameObjectName); i++)
    { 
        gameObjectName[i] = gameObject.getName()[i]; 
    } 

    if (ImGui::InputText("name", gameObjectName, IM_ARRAYSIZE(gameObjectName)))
    {
        std::string newName = gameObjectName;
        if (!newName.empty())
        {
            gameObject.setName(newName.c_str());
        }
    }

    Engine::Core::Maths::Vec3 pos = gameObject.getPosition();
    Engine::Core::Maths::Vec3 rot = gameObject.getRotation();
    Engine::Core::Maths::Vec3 scale = gameObject.getScale();

    ImGui::Columns(4);
    ImGui::Text("Position :");
    ImGui::NextColumn();
    ImGui::DragFloat("X ", &pos.x, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Y ", &pos.y, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Z ", &pos.z, 0.1f);

    ImGui::Columns(1); ImGui::Columns(4);
    ImGui::Text("Rotation :");
    ImGui::NextColumn();
    ImGui::DragFloat("X ", &rot.x, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Y ", &rot.y, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Z ", &rot.z, 0.1f);

    ImGui::Columns(1); ImGui::Columns(4);
    ImGui::Text("Scale :");
    ImGui::NextColumn();
    ImGui::DragFloat("X ", &scale.x, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Y ", &scale.y, 0.1f);
    ImGui::NextColumn();
    ImGui::DragFloat("Z ", &scale.z, 0.1f);
    ImGui::Columns(1);

    gameObject.setTranslation(pos);
    gameObject.setRotation(rot);
    gameObject.setScale(scale);

    ImGui::End();

    return !open;
}