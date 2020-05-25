#include "GE/LowRenderer/EditorTools/Editor.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Ressources/Saves.hpp"
#include "GE/LowRenderer/EditorTools/InspectorWindow.hpp"
#include "GE/LowRenderer/EditorTools/EditModeControllerWindow.hpp"
#include "GE/LowRenderer/Light/light.hpp"
#include "imgui/imgui.h"
#include "GE/LowRenderer/camera.hpp"

#include <string>

using namespace Engine::LowRenderer;
using namespace Engine::LowRenderer::EditorTools;
using namespace Engine::Core::InputSystem;
using namespace Engine::Ressources;
using namespace Engine;
using namespace Engine::Ressources::Save;

bool Editor::_enable = false;

void Editor::updateInput()
{
    if (Input::keyboard.getKeyState(SDL_SCANCODE_F1) == E_KEY_STATE::TOUCHED)
    {
        _enable = !_enable;

        SDL_SetRelativeMouseMode(_enable ? SDL_FALSE : SDL_TRUE);
        SDL_ShowCursor(_enable);
    }

    if (Input::keyboard.getKeyState(SDL_SCANCODE_F8) == E_KEY_STATE::DOWN)
    {
        ImGui::ShowDemoWindow();
    }
}

void Editor::quickSaveSceneOption(Scene& scene, GE& engine)
{
    if (ImGui::MenuItem("Save scene", "CTRL+S"))
    {
        std::string savePath (SAVE_FOLDER_PATH);
        savePath += "QuickSave";
        savePath += std::to_string(engine.savePaths.size());
        savePath += ".xml";

        saveScene(scene, engine, savePath.c_str());
    }
}

void Editor::loadSceneOption(Scene& scene, GE& engine)
{
    if (ImGui::BeginMenu("Load scene", "CTRL+L"))
    {
        for (auto &&i : engine.savePaths)
        {
            if(ImGui::MenuItem(i.substr(19, i.size() - 23).c_str()))
            {
                Light::resetLight();
                Scene::_currentScene->reset();
                *Scene::_currentScene = std::make_unique<Scene>();
                setupScene(*Scene::getCurrentScene(), engine, i.c_str());
                Camera::setCamUse(static_cast<Camera*>(&Scene::getCurrentScene()->getGameObject("world/MainCamera")));
                ImGui::EndMenu();
                return;
            }
        }
        ImGui::EndMenu();
    }
}

void Editor::update(Scene& scene, GE& engine)
{
    updateInput ();

    if (!_enable)
        return;
        
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Edit"))
        {
            quickSaveSceneOption(scene, engine);
            loadSceneOption(scene, engine);
            ImGui::Separator();

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    SceneGraphWindow::update(scene);

    if (SceneGraphWindow::pTargetGameObject != nullptr)
    {
        if (InspectorWindow::isCloseAfterUpdate(*SceneGraphWindow::pTargetGameObject))
        {
            SceneGraphWindow::pTargetGameObject = nullptr;
        }
    }

    EditModeControllerWindow::update();
}