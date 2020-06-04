#ifndef DNEDITOR

#include "GE/LowRenderer/EditorTools/InspectorWindow.hpp"
#include "GE/LowRenderer/EditorTools/EditModeControllerWindow.hpp"
#include <typeinfo>

using namespace Engine::Ressources;
using namespace Engine::LowRenderer::EditorTools;

InspectorWindow::TransformMode InspectorWindow::transformMode {InspectorWindow::TransformMode::Local};
        
void InspectorWindow::updateTransform(GameObject& gameObject)
{
    if (!ImGui::CollapsingHeader("Transform"))
        return;
    
    Engine::Core::Maths::Vec3 pos;
    Engine::Core::Maths::Vec3 rot;
    Engine::Core::Maths::Vec3 scale;

    const char* modes_names[InspectorWindow::TransformMode::Count] = {"Local", "Gloabl (read only)"};
    const char* mode_name = (transformMode >= 0 && transformMode < InspectorWindow::TransformMode::Count) ? modes_names[transformMode] : "Unknown";
    ImGui::SliderInt("##transformMode", (int*)&transformMode, 0, InspectorWindow::TransformMode::Count - 1, mode_name);

    ImGui::SameLine();
    if (ImGui::Checkbox("Lock", &EditModeControllerWindow::lockElement))
    {
        EditModeControllerWindow::lookAtTowardTargetOnEditMode(gameObject);
    }

    switch (transformMode)
    {
        case InspectorWindow::TransformMode::Local:
        pos = gameObject.getPosition();
        rot = gameObject.getRotation() * 180 / M_PI;
        scale = gameObject.getScale();

        break;

        case InspectorWindow::TransformMode::Global:

        pos = gameObject.getGlobalPosition();
        rot = gameObject.getRotation() * 180 / M_PI;
        scale = gameObject.getGlobalScale();

        break;
    
    default:

        pos = Engine::Core::Maths::Vec3::zero;
        rot = Engine::Core::Maths::Vec3::zero;
        scale = Engine::Core::Maths::Vec3::zero;

        break;
    }

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

    switch (transformMode)
    {
        case InspectorWindow::TransformMode::Local :

        gameObject.setTranslation(pos);
        gameObject.setRotation(rot * M_PI / 180.f);
        gameObject.setScale(scale);

        break;

        default:
        break;
    }
}


void InspectorWindow::updateHeader(GameObject& gameObject)
{
    char gameObjectName[128]; //Max name lenght

    for (size_t i = 0; i < sizeof(gameObjectName); i++)
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
        if (ImGui::CollapsingHeader(i->toString().c_str()))
        {
            i->serializeOnEditor();
        }
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

#endif