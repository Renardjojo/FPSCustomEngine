//Project : Engine
//Editing by Six Jonathan
//Date : 08/01/2020 - 18 h 22

#include <memory>
#include "GE/Core/Rendering/Renderer/iRenderer.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Ressources/type.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/ressourcesManager.hpp"
#include "GE/Ressources/mesh.hpp"
#include "GE/Ressources/shader.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Ressources/sample.hpp"
#include "GE/Ressources/music.hpp"
#include "GE/Ressources/font.hpp"
#include "GE/Ressources/ui.hpp"
#include "Game/define.h"

#ifndef _GE_H
#define _GE_H

namespace Engine
{
    class GE
    {
        public:

            #pragma region constructor/destructor

            GE  ();
            GE  (const GE& other)    = delete;
            ~GE ();

            #pragma endregion //!constructor/destructor

            #pragma region methods

            void pollEvent              () { Core::InputSystem::Input::pollEvent(ren_->getWin()->getId());}
            void updateTime             () { Core::System::TimeSystem::update();}
            void clearRenderer          () { ren_->clear(); }
            void renderPresent          () { ren_->draw();  }

            #pragma endregion //!methods

            #pragma region accessor

            const Engine::Ressources::Size getWinSize () { return ren_->getWin()->getSize(); }
            Engine::Core::Renderer::IRenderer* getRenderer () { return ren_.get(); }

            float   getDeltaTime() { return Core::System::TimeSystem::getDeltaTime(); };

            #pragma endregion //!accessor

        public:

            #pragma region attribut

            std::unique_ptr<Core::Renderer::IRenderer>  ren_;

            Engine::Ressources::t_RessourcesManager ressourceManager_;

            E_GAME_STATE gameState;

            std::vector<std::string> savePaths;

            #pragma endregion //!attribut

        private:
    };
}//namespace Engine

#endif // _GE_H