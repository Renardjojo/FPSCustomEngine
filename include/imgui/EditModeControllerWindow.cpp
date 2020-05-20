#include "GE/LowRenderer/EditorTools/EditModeControllerWindow.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Core/Maths/vec.hpp"

using namespace Engine::LowRenderer::EditorTools;
using namespace Engine::Core::System;
using namespace Engine::Core::InputSystem;
using namespace Engine::Core::Maths;
using namespace Engine::Ressources;

void EditModeControllerWindow::update()
{
    ImGui::Begin("Edit Mode Controller", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);

    float timeScale = TimeSystem::getTimeScale();

    const char* modes_names[Mode::ModeCount] = { "EditMode", "GameMode"};
    const char* mode_name = (mode >= 0 && mode < Mode::ModeCount) ? modes_names[mode] : "Unknown";
    if (ImGui::SliderInt("##camMode", (int*)&mode, 0, Mode::ModeCount - 1, mode_name))
    {
        switch (mode)
        {
        case EditMode:
            pGameCamera = Engine::LowRenderer::Camera::getCamUse();

            if (pGameCamera != nullptr)
            {
                pEditModeCamera->setAspect(ImGui::GetIO().DisplaySize.x / ImGui::GetIO().DisplaySize.y);
                pEditModeCamera->use();
            }
            break;

        case GameMode:
            if (pGameCamera != nullptr)
            {
                pGameCamera->use();
            }
            break;
        
        default:
            break;
        }
    }

    if (mode == EditMode)
    {
        updateEditModeCameraControl();
    }

    ImGui::Text("Time scale :");
    ImGui::SliderFloat("##timeScale", &timeScale, 0.f, 5.f);

    TimeSystem::setTimeScale(timeScale);

    ImGui::End();
}

void EditModeControllerWindow::updateEditModeCameraControl()
{
   if (Input::keyboard.isDown[Input::keyboard.up])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getUnscaledDetlaTime());
        pEditModeCamera->translate(vec);
    }

    if (Input::keyboard.isDown[Input::keyboard.down])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getUnscaledDetlaTime());
        pEditModeCamera->translate(-vec);
    }

    if (Input::keyboard.isDown[Input::keyboard.jump])
    {
        pEditModeCamera->translate({0.f, 20.f * TimeSystem::getUnscaledDetlaTime(), 0.f});
    }

    if (Input::keyboard.isDown[SDL_SCANCODE_LCTRL])
    {
        pEditModeCamera->translate({0.f, -20.f * TimeSystem::getUnscaledDetlaTime(), 0.f});
    }

    if (Input::keyboard.isDown[Input::keyboard.left])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-Input::mouse.motion.x * 0.1f * TimeSystem::getUnscaledDetlaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getUnscaledDetlaTime());
        pEditModeCamera->translate(-playerDirOrtho);
    }

    if (Input::keyboard.isDown[Input::keyboard.right])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-Input::mouse.motion.x * 0.1f * TimeSystem::getUnscaledDetlaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getUnscaledDetlaTime());
        pEditModeCamera->translate(playerDirOrtho);
    }

    if (Input::mouse.motion.y != 0)
    {
        pEditModeCamera->rotate(-Input::mouse.motion.y * 0.5f * TimeSystem::getUnscaledDetlaTime(), {1.f, 0.f, 0.f});
    }

    if (Input::mouse.motion.x != 0)
    {
            Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
            vecDirPlayer.rotate(Input::mouse.motion.x * 0.5f * TimeSystem::getUnscaledDetlaTime() * 180 / M_PI);
            dirCamera.x = vecDirPlayer.x;
            dirCamera.z = vecDirPlayer.y;
            pEditModeCamera->rotate(-Input::mouse.motion.x * 0.5f * TimeSystem::getUnscaledDetlaTime(), {0.f, 1.f, 0.f});
    }

    static int exFrameWheelVal = Input::mouse.wheel_scrolling;

    if (Input::mouse.wheel_scrolling != exFrameWheelVal)
    {
        if(Input::mouse.wheel_scrolling > exFrameWheelVal)
        {
            pEditModeCamera->setFovY(pEditModeCamera->getProjectionInfo().fovY + 5);
        }
        else
        {
            pEditModeCamera->setFovY(pEditModeCamera->getProjectionInfo().fovY - 5);
        }
        
        exFrameWheelVal = Input::mouse.wheel_scrolling;
    }

    pEditModeCamera->update();
}

void EditModeControllerWindow::lookAtTowardTargetOnEditMode(const GameObject& gameObject)
{
    if (mode == EditMode)
    {
        /*Focus on element*/
        pEditModeCamera->lookAt(gameObject.getGlobalPosition() + (pEditModeCamera->getPosition() - gameObject.getGlobalPosition()).getNormalize() * focusRange, gameObject.getGlobalPosition(), Vec3::up);
    }
}

Engine::LowRenderer::Camera*                 EditModeControllerWindow::pGameCamera      {nullptr};
std::unique_ptr<Engine::LowRenderer::Camera> EditModeControllerWindow::pEditModeCamera  {std::make_unique<Camera>(CameraPerspectiveCreateArg{{0.f, 0.f, 30.f}, {0.f, 0.f, 0.f}, 600.f / static_cast<float>(800.f), 0.1f, 10000.0f, 45.0f, "EditModeCamera"})};
Mode                                         EditModeControllerWindow::mode             {Mode::GameMode};
Engine::Core::Maths::Vec3                    EditModeControllerWindow::dirCamera        {0.f, 0.f, -1.f};
float                                        EditModeControllerWindow::focusRange       {15.f};
