//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-15 - 09 h 38

#ifndef _EDIT_MODE_CONTROLLER_WINDOW_H
#define _EDIT_MODE_CONTROLLER_WINDOW_H

#include "imgui/imgui.h"
//#include "GE/Core/DataStructure/graph.hpp"
#include "GE//LowRenderer/camera.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"

namespace Engine::LowRenderer::EditorTools
{
    enum Mode
    { 
        EditMode,
        GameMode,
        Element_COUNT
    };

    class EditModeControllerWindow
    {
        public:
    
        #pragma region constructor/destructor
    
        EditModeControllerWindow ()					                                = delete;
        EditModeControllerWindow (const EditModeControllerWindow& other)		    = delete;
        EditModeControllerWindow (EditModeControllerWindow&& other)				    = delete;
        virtual ~EditModeControllerWindow ()				                        = delete;
        EditModeControllerWindow& operator=(EditModeControllerWindow const& other)	= delete;
        EditModeControllerWindow& operator=(EditModeControllerWindow && other)		= delete;
    
        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        static void update();
        static void updateEditeModeCameraControlle();
        static void lookAtTowardTargetOnEditMode(const Engine::Ressources::GameObject& gameObject);

        #pragma endregion //!methods
    
        #pragma region accessor
        #pragma endregion //!accessor
    
        #pragma region mutator
        #pragma endregion //!mutator
    
        #pragma region operator
        #pragma endregion //!operator
    
        #pragma region convertor
        #pragma endregion //!convertor
    
        static Engine::LowRenderer::Camera*                 pGameCamera;
        static std::unique_ptr<Engine::LowRenderer::Camera> pEditModeCamera;
        static Mode                                         mode;
        static Engine::Core::Maths::Vec3                    dirCamera;
        static float                                        focusRange;

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

#endif //_EDIT_MODE_CONTROLLER_WINDOW_H