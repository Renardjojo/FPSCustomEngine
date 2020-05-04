#include "GE/Core/System/UISystem.hpp"
#include "GE/Core/Debug/log.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/GE.hpp"

using namespace Engine::Ressources;
using namespace Engine::Core::Systems;
using namespace Engine::Core::Time;
using namespace Engine::Core::Debug;

std::vector<Button *> UISystem::pButtons;
std::vector<TextField *> UISystem::pTextFields;
TextField * UISystem::pActiveTextfield;

bool UISystem::isActive = true;

void UISystem::update(Engine::GE& gameEngine) noexcept
{
    if (!isActive)
        return;

    int state = gameEngine.inputManager_.mouse.oneLeftClick();

    for (Button *button : pButtons)
    {
        if (!button->isActive || button->whenIsActive != gameEngine.gameState)
                continue;
        button->isButtonPressed(gameEngine.inputManager_.mouse.position.x, gameEngine.inputManager_.mouse.position.y, state);
    }



    bool atLeastOneTextfieldActive = false;

    for (TextField *textField : pTextFields)
    {
        if (gameEngine.inputManager_.mouse.leftClic_down 
        &&  textField->isTextFieldactive(gameEngine.inputManager_.mouse.position.x, gameEngine.inputManager_.mouse.position.y))
        {
            pActiveTextfield = textField;
            atLeastOneTextfieldActive = true;
        }
    }
    
    if (!atLeastOneTextfieldActive)
        pActiveTextfield = nullptr;

    if (pActiveTextfield && gameEngine.inputManager_.keyboard.isDown)
        pActiveTextfield->stringDel();

    if (pActiveTextfield)
    {
        pActiveTextfield->updateString(static_cast<char>(gameEngine.inputManager_.keyboard.key));
    }

}

void UISystem::draw(Engine::GE& gameEngine) noexcept
{
    for (Button *button : pButtons)
    {
        if (!button->isActive || button->whenIsActive != gameEngine.gameState)
                continue;
        button->draw();
    }
    for (TextField *textField : pTextFields)
    {
        textField->draw();
    }
}