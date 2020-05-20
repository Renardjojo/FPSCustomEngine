#include "GE/LowRenderer/EditorTools/Editor.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/LowRenderer/EditorTools/InspectorWindow.hpp"
#include "GE/LowRenderer/EditorTools/EditModeControllerWindow.hpp"
#include "imgui/imgui.h"

using namespace Engine::LowRenderer::EditorTools;
using namespace Engine::Core::InputSystem;

bool Editor::_enable = false;

void Editor::updateInput()
{
    if (Input::keyboard.onePressed(SDL_SCANCODE_F1) == 1)
    {
        _enable = !_enable;

       //SDL_SetRelativeMouseMode(!_enable ? SDL_FALSE : SDL_TRUE);
        SDL_ShowCursor(_enable);
    }
}

void Editor::update(Engine::Core::DataStructure::Graph& graph)
{
    updateInput ();

    if (!_enable)
        return;
        
    SceneGraphWindow::update(graph);

    if (SceneGraphWindow::pTargetGameObject != nullptr)
    {
        if (InspectorWindow::isCloseAfterUpdate(*SceneGraphWindow::pTargetGameObject))
        {
            SceneGraphWindow::pTargetGameObject = nullptr;
        }
    }

    EditModeControllerWindow::update();

}