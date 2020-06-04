#ifndef DNEDITOR

#include "GE/LowRenderer/EditorTools/SceneGraphWindow.hpp"
#include "GE/LowRenderer/EditorTools/EditModeControllerWindow.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::DataStructure;
using namespace Engine::LowRenderer::EditorTools;

void SceneGraphWindow::recursifTreeCreation(GameObject& gameObject)
{
    if (gameObject.children.empty())
    {
        if (ImGui::SmallButton(gameObject.getCName()))
        {
            pTargetGameObject = &gameObject;
            EditModeControllerWindow::lookAtTowardTargetOnEditMode(gameObject);
        }
    }
    else
    {
        if (ImGui::TreeNode(gameObject.getCName()))
        {   
            ImGui::SameLine();
            if (ImGui::SmallButton(gameObject.getCName()))
            {
                pTargetGameObject = &gameObject;
                EditModeControllerWindow::lookAtTowardTargetOnEditMode(gameObject);
            }

            for (auto&& child : gameObject.children)
            {
                recursifTreeCreation(*child.get());
            }
            ImGui::TreePop();
        }
    }
}

void SceneGraphWindow::update(Graph& graph)
{
    ImGui::Begin("Scene Graph");                          // Create a window called "Hello, world!" and append into it.

    recursifTreeCreation(graph.getWorld());

    ImGui::End();
}

GameObject* SceneGraphWindow::pTargetGameObject = nullptr;

#endif