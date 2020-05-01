

#ifndef _UISYSTEM_H
#define _UISYSTEM_H

#include <vector>
#include "GE/Ressources/ui.hpp"
#include "GE/Core/InputSystem/input.hpp"

namespace Engine::Core::Systems
{
    class UISystem
    {

    protected:

        static std::vector<Engine::Ressources::Button *> pButtons;
        static std::vector<Engine::Ressources::TextField *> pTextFields;

        static Engine::Ressources::TextField* pActiveTextfield;


    public:

        UISystem() = delete;
        UISystem(const UISystem &other) = delete;
        UISystem(UISystem &&other) = delete;
        virtual ~UISystem() = delete;
        UISystem &operator=(UISystem const &) = delete;
        UISystem &operator=(UISystem &&) = delete;
        
        static bool isActive;

        static void update(Engine::Core::InputSystem::Input input) noexcept;

        static void draw() noexcept;

        static void addTextField(Engine::Ressources::TextField *pTextField) noexcept
        {
            pTextFields.push_back(pTextField);
        }

        static void updateTextFieldPointor(Engine::Ressources::TextField *newPointorTextField, Engine::Ressources::TextField *exPointorTextField) noexcept
        {
            for (std::vector<Engine::Ressources::TextField *>::iterator it = pTextFields.begin(); it != pTextFields.end(); it++)
            {
                if ((*it) == exPointorTextField)
                {
                    *it = newPointorTextField;
                    return;
                }
            }
        }

        static void removeTextField(Engine::Ressources::TextField *pTextField) noexcept
        {
            for (std::vector<Engine::Ressources::TextField *>::iterator it = pTextFields.begin(); it != pTextFields.end(); it++)
            {
                if ((*it) == pTextField)
                {
                    pTextFields.erase(it);
                    return;
                }
            }
        }

        static void addButton(Engine::Ressources::Button *pButton) noexcept
        {
            pButtons.push_back(pButton);
        }

        static void updateButtonPointor(Engine::Ressources::Button *newPointorButton, Engine::Ressources::Button *exPointorButton) noexcept
        {
            for (std::vector<Engine::Ressources::Button *>::iterator it = pButtons.begin(); it != pButtons.end(); it++)
            {
                if ((*it) == exPointorButton)
                {
                    *it = newPointorButton;
                    return;
                }
            }
        }

        static void removeButton(Engine::Ressources::Button *pButton) noexcept
        {
            for (std::vector<Engine::Ressources::Button *>::iterator it = pButtons.begin(); it != pButtons.end(); it++)
            {
                if ((*it) == pButton)
                {
                    pButtons.erase(it);
                    return;
                }
            }
        }
    };
}

#endif //_UISYSTEM_H