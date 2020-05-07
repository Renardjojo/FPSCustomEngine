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
Button * UISystem::pOverredButton;
Engine::Core::Maths::Vec2 UISystem::keyboardXY;
bool UISystem::isUsingKeyboard = false;

bool UISystem::isActive = true;

void UISystem::update(Engine::GE& gameEngine) noexcept
{
    if (!isActive)
        return;

    if (gameEngine.inputManager_.keyboard.isTouch && !isUsingKeyboard)
    {
        isUsingKeyboard = true;
        Button* tempButton = nullptr;
        for (Button *button : pButtons)
        {
            if (button->whenIsActive == gameEngine.gameState)
            {
                if (!tempButton || tempButton->getPos().length() > button->getPos().length())
                    tempButton = button;
            }
        }
        if (!tempButton)
        {
            isUsingKeyboard = false;
            return;
        }
        keyboardXY = tempButton->getPos();
        pOverredButton = tempButton;
    }
    if (gameEngine.inputManager_.mouse.isTouch)
    {
        isUsingKeyboard = false;
    }

    if (isUsingKeyboard)
    {
        int state = 3;
        
        Button* tempButton = nullptr;

        if (gameEngine.inputManager_.keyboard.onePressed(SDL_SCANCODE_DOWN) == 1)
        {
            for (Button *button : pButtons)
            {
                if (button->whenIsActive == gameEngine.gameState && button != pOverredButton)
                {
                    if  ((tempButton == nullptr && button->getPos().y > pOverredButton->getPos().y) 
                    ||   (button->getPos().y > pOverredButton->getPos().y && button->getPos().y < tempButton->getPos().y))
                    {
                        tempButton = button;
                        keyboardXY = button->getPos();
                    }
                }
            }
        }
        if (gameEngine.inputManager_.keyboard.onePressed(SDL_SCANCODE_UP) == 1)
        {
            for (Button *button : pButtons)
            {
                if (button->whenIsActive == gameEngine.gameState && button != pOverredButton)
                {
                    if  ((tempButton == nullptr && button->getPos().y < pOverredButton->getPos().y) 
                    ||   (button->getPos().y < pOverredButton->getPos().y && button->getPos().y > tempButton->getPos().y))
                    {
                        tempButton = button;
                        keyboardXY = button->getPos();
                    }
                }
            }
        }

        if (gameEngine.inputManager_.keyboard.onePressed(SDL_SCANCODE_RETURN) == 1)
            state = 1;

        pOverredButton->isButtonPressed(-100, -100, 0);
        if (tempButton)
            pOverredButton = tempButton;    
        pOverredButton->isButtonPressed(keyboardXY.x, keyboardXY.y, state);
    }   
    else
    {
        int state = gameEngine.inputManager_.mouse.oneLeftClick();

        for (Button *button : pButtons)
        {
            if (!button->isActive || button->whenIsActive != gameEngine.gameState)
                    continue;
            button->isButtonPressed(gameEngine.inputManager_.mouse.position.x, gameEngine.inputManager_.mouse.position.y, state);
        }   
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