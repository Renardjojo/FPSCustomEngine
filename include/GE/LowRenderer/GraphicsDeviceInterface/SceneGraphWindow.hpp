//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-15 - 09 h 38

#ifndef _SCENE_GRAPH_WINDOW_H
#define _SCENE_GRAPH_WINDOW_H

#include "imgui/imgui.h"
#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer::GraphicsDeviceInterface
{
    class SceneGraphWindow
    {
        public:
    
        #pragma region constructor/destructor
    
        SceneGraphWindow ()					                        = default;
        SceneGraphWindow (const SceneGraphWindow& other)		    = default;
        SceneGraphWindow (SceneGraphWindow&& other)				    = default;
        virtual ~SceneGraphWindow ()				                = default;
        SceneGraphWindow& operator=(SceneGraphWindow const& other)	= default;
        SceneGraphWindow& operator=(SceneGraphWindow && other)		= default;
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        static void recursifTreeCreation(const Engine::Ressources::GameObject& gameObject)
        {
            if (gameObject.children.empty())
            {
                ImGui::Text(gameObject.getName());
            }
            else
            {
                if (ImGui::TreeNode(gameObject.getName()))
                {
                    for (auto&& child : gameObject.children)
                    {
                        recursifTreeCreation(*child.get());
                    }
                    ImGui::TreePop();
                }
            }
        }

        static void update(const Engine::Core::DataStructure::Graph& graph)
        {
            ImGui::Begin("Scene Graph");                          // Create a window called "Hello, world!" and append into it.

            recursifTreeCreation(graph.getWorld());

            ImGui::End();
        }

        #pragma endregion //!methods
    
        #pragma region accessor
        #pragma endregion //!accessor
    
        #pragma region mutator
        #pragma endregion //!mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor
    
        protected:
    
        #pragma region attribut
        #pragma endregion //!attribut
    
        #pragma region static attribut
        #pragma endregion //! static attribut
    
        #pragma region methods
        #pragma endregion //!methods
    
        private:
    
    };

} // !namespace Engine::LowRenderer::GraphicsDeviceInterface

#endif //_SCENE_GRAPH_WINDOW_H