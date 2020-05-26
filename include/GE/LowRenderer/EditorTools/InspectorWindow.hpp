//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-15 - 09 h 38

#ifndef _INSPECTOR_WINDOW_H
#define _INSPECTOR_WINDOW_H

#include "imgui/imgui.h"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Ressources/GameObject.hpp"

#include <string>

namespace Engine::LowRenderer::EditorTools
{
    class InspectorWindow
    {
        enum TransformMode
        { 
            Local,
            Global,
            Count
        };

        protected : 

        static TransformMode transformMode;

        static
        void updateTransform(Engine::Ressources::GameObject& gameObject);

        static
        void updateHeader(Engine::Ressources::GameObject& gameObject);

        static
        void updateComponent(Engine::Ressources::GameObject& gameObject);

        public:
    
        #pragma region constructor/destructor
    
        InspectorWindow ()					                        = delete;
        InspectorWindow (const InspectorWindow& other)		        = delete;
        InspectorWindow (InspectorWindow&& other)				    = delete;
        virtual ~InspectorWindow ()				                    = delete;
        InspectorWindow& operator=(InspectorWindow const& other)	= delete;
        InspectorWindow& operator=(InspectorWindow && other)		= delete;
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        static
        bool isCloseAfterUpdate(Engine::Ressources::GameObject& gameObject);

        #pragma endregion //!methods
    
        #pragma region accessor
        #pragma endregion //!accessor
    
        #pragma region mutator
        #pragma endregion //!mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor   
    };

} // !namespace Engine::LowRenderer::Editor

#endif //_INSPECTOR_WINDOW_H