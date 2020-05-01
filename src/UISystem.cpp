#include "GE/Core/System/UISystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/System/TimeSystem.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::Systems;
using namespace Engine::Core::Time;
using namespace Engine::Core::Debug;

std::vector<Button *> UISystem::pButtons;
std::vector<TextField *> UISystem::pTextFields;
TextField * UISystem::pActiveTextfield;

bool UISystem::isActive = true;

void UISystem::update(Engine::Core::InputSystem::Input input) noexcept
{
    if (!isActive)
        return;

    int state = input.mouse.leftClic_down;

    for (Button *button : pButtons)
    {
        button->isButtonPressed(input.mouse.position.x, input.mouse.position.y, state);
    }

    bool atLeastOneTextfieldActive = false;

    for (TextField *textField : pTextFields)
    {
        if (textField->isTextFieldactive(input.mouse.position.x, input.mouse.position.y))
        {
            pActiveTextfield = textField;
            atLeastOneTextfieldActive = true;
        }
    }
    
    if (!atLeastOneTextfieldActive)
        pActiveTextfield = nullptr;

    if (pActiveTextfield && input.keyboard.isDown[SDL_SCANCODE_BACKSPACE])
        pActiveTextfield->stringDel();
}

void UISystem::draw() noexcept
{
    for (Button *button : pButtons)
    {
        button->draw();
    }
    for (TextField *textField : pTextFields)
    {
        textField->draw();
    }
}