//Project : Engine
//Editing by Six Jonathan
//Date : 08/01/2020 - 18 h 22

#include <memory>
#include "GE/Core/Rendering/Renderer/iRenderer.hpp"
#include "GE/Core/InputSystem/input.hpp"
#include "GE/Resources/type.hpp"
#include "GE/Core/TimeSystem/time.hpp"
#include "GE/Resources/resourcesManager.hpp"
#include "GE/Resources/mesh.hpp"
#include "GE/Resources/shader.hpp"
#include "GE/LowRenderer/model.hpp"
#include "GE/Resources/sample.hpp"
#include "GE/Resources/music.hpp"
#include "GE/Resources/font.hpp"

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

            void pollEvent              () { inputManager_.pollEvent(ren_->getWin()->getId());}
            void updateTime             () { Engine::Core::TimeSystem::TimeSystem::update();}
            void clearRenderer          () { ren_->clear(); }
            void renderPresent          () { ren_->draw();  }

            #pragma endregion //!methods

            #pragma region accessor

            const Engine::Core::InputSystem::T_windowEvent&	    getWindowEvent() const noexcept      { return inputManager_.window;};
            const Engine::Core::InputSystem::T_inputMouse&	    getMouseEvent() const noexcept       { return inputManager_.mouse;};
            const Engine::Core::InputSystem::T_inputKeyboard&	getKeyboardEvent() const noexcept    { return inputManager_.keyboard; };

            const Engine::Resources::Size getWinSize () { return ren_->getWin()->getSize(); }

            float   getDeltaTime() { return Engine::Core::TimeSystem::TimeSystem::getDeltaTime(); };

            #pragma endregion //!accessor

            #pragma region mutator
            #pragma endregion //!mutator

            #pragma region operator
            #pragma endregion //!operator

            #pragma region convertor
            #pragma endregion //!convertor

        public:

            #pragma region attribut

            std::unique_ptr<Engine::Core::Renderer::IRenderer>  ren_;
            Engine::Core::InputSystem::Input                    inputManager_;

            Engine::Resources::ResourcesManager<Engine::Resources::Mesh, 
                                                Engine::Resources::Shader, 
                                                Engine::Resources::Material, 
                                                Engine::Resources::Sample, 
                                                Engine::Resources::Music, 
                                                Engine::Resources::Font>    resourceManager_;

            #pragma endregion //!attribut

            #pragma region methods
            #pragma endregion //!methods

        private:
    };
}//namespace Engine

#endif // _GE_H