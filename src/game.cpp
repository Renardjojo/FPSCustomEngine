#include "Game/game.hpp"
#include "Game/demo.hpp"
#include "imgui/imgui.h"
#include "GE/Core/System/TimeSystem.hpp"

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace Engine::Ressources;
using namespace Engine;
using namespace Engine::Core::System;

Game::Game::Game (Engine::GE& gameEngine)
    : gameEngine_ {gameEngine}
{}

void Game::Game::run ()
{
    Demo demo(gameEngine_);

bool show_demo_window = false;
bool show_another_window = false;
float clear_color;

    while(gameEngine_.gameState != E_GAME_STATE::EXIT)
    {   
        TimeSystem::update([&]()
                            { 
                                demo.update();
                                gameEngine_.pollEvent();
                            }, 
                            [&]() 
                            {
                                ImGui_ImplOpenGL3_NewFrame();
                                ImGui_ImplSDL2_NewFrame((SDL_Window*)gameEngine_.getRenderer()->getWin()->get());
                                ImGui::NewFrame();

                                if (show_demo_window)
                                    ImGui::ShowDemoWindow(&show_demo_window);

                                // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
                                {
                                    static float f = 0.0f;
                                    static int counter = 0;

                                    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                                    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                                    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                                    ImGui::Checkbox("Another Window", &show_another_window);

                                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                                    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                                    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                                        counter++;
                                    ImGui::SameLine();
                                    ImGui::Text("counter = %d", counter);

                                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                                    ImGui::End();
                                }
                            },
                            [&]()
                            {      
                                ImGui::Render();
                                gameEngine_.clearRenderer();
                                demo.display();
                                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                                gameEngine_.renderPresent();
                            });
    }
}