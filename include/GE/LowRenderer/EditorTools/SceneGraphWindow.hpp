//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-15 - 09 h 38

#ifndef _SCENE_GRAPH_WINDOW_H
#define _SCENE_GRAPH_WINDOW_H

#include "imgui/imgui.h"
#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer::EditorTools
{
    class SceneGraphWindow
    {
        public:
    
        #pragma region constructor/destructor
    
        SceneGraphWindow ()					                        = delete;
        SceneGraphWindow (const SceneGraphWindow& other)		    = delete;
        SceneGraphWindow (SceneGraphWindow&& other)				    = delete;
        virtual ~SceneGraphWindow ()				                = delete;
        SceneGraphWindow& operator=(SceneGraphWindow const& other)	= delete;
        SceneGraphWindow& operator=(SceneGraphWindow && other)		= delete;
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        static void recursifTreeCreation(Engine::Ressources::GameObject& gameObject);

        static void update(Engine::Core::DataStructure::Graph& graph);

        #pragma endregion //!methods
    
        #pragma region accessor
        #pragma endregion //!accessor
    
        #pragma region mutator
        #pragma endregion //!mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor
    
        static Engine::Ressources::GameObject* pTargetGameObject;

        protected:
    
        #pragma region attribut

        #pragma endregion //!attribut
    
        #pragma region static attribut
        #pragma endregion //! static attribut
    
        #pragma region methods
        #pragma endregion //!methods
    
        private:
    
    };

} // !namespace Engine::LowRenderer::Editor

#endif //_SCENE_GRAPH_WINDOW_H