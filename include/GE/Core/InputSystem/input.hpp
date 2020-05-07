//Project : rasterizer
//Editing by Six Jonathan
//Date : 21/11/2019 - 09 h 59

#ifndef _GE_INPUT_MANAGER_H
#define _GE_INPUT_MANAGER_H

#include <SDL2/SDL_events.h>
#include <iostream>

//function based on SDL2 library. See documentation on : https://wiki.libsdl.org/SDL_Scancode to use SDL_Scancode for keyboard
//Else for all documentation of input possibel see : https://wiki.libsdl.org/SDL_Event

namespace Engine::Core::InputSystem
{
    typedef struct S_inputMouse
    {
        //mouse input
        SDL_Point		position;
        SDL_Point 		motion;
        SDL_Point		wheel_value;
        int 			wheel_scrolling;
        unsigned int	clicks;
        float 			timeFirstLeftClic;	//implementation data
        bool 			wheelClic;
        bool 			isTouch;
        bool 			rightClic_down;
        bool 			doubleLeftClic;	//if interval of 2click is under 0.2s
        bool 			leftClic_down;
        bool            leftClicPressed;

        int oneLeftClick()
        {
            if (leftClic_down && !leftClicPressed)
            {
                leftClicPressed = true;
                return 1;
            }
            else if (leftClic_down && leftClicPressed)
            {
                leftClicPressed = true;
                return 2;
            }
            else if (!leftClic_down)
            {
                leftClicPressed = false;
                return 0;
            }
            else
                return 0;
        }

    } T_inputMouse;

    typedef struct S_inputKeyboard
    {	
        bool 		isTouch;
        bool 		isDown[SDL_NUM_SCANCODES]; //use Scancode enum in file SDL_Scancode.
        bool        isPressed[SDL_NUM_SCANCODES];
        bool		flagEscIsRelease;
        bool		escIsRelease;
        SDL_Keycode key;

        int onePressed(const SDL_Scancode key)
        {
            if (isDown[key] && !isPressed[key])
            {
                isPressed[key] = true;
                return 1;
            }
            else if (isDown[key] && isPressed[key])
            {
                isPressed[key] = true;
                return 2;
            }
            else if (!isDown[key])
            {
                isPressed[key] = false;
                return 0;
            }
            else
                return 0;
        }

    } T_inputKeyboard;

    typedef struct S_windowEvent
    {	
        bool windowEventHappend; //true if event append
        
        bool isShow;	//if window is show or hidden
        bool isExposed; //window has been exposed and should be redrawn
        bool isMoved; 	//window has been moved.  Note : use SDL_GetWindowPosition to know them new data
        bool isResized; //window has been resized. Note : use SDL_GetWindowSize to know them new data
        bool isMinimized;	// window has been minimized. Note use SDL_MinimizeWindow
        bool isMaximized;	// window has been maximized. Note use SDL_MaximizeWindow
        bool focusInWindow; //true while mouse or keyboard focus is in focus

    } T_windowEvent;

    class Input
    {
        public:
            
        Input ();
        Input (const Input& other)  = default;
        virtual ~Input ()           = default;

            /*----------*/
            /* methode  */
            /*----------*/

            /**
            * function : pollEvent
            *
            * parameter :
            *  Uint32 windowID : the id of current window
            *
            * return (type void):
            *
            * brief : this function catch event and update class input. Do once by frame
            */
            void		pollEvent		(Uint32 windowID);

            /*----------*/
            /* accessor */
            /*----------*/

            /*----------*/
            /* mutator  */
            /*----------*/

            /*----------*/
            /* operator */
            /*----------*/

            /*----------*/
            /* convertor*/
            /*----------*/

            //public variable (get and set with no effect for class)
            T_windowEvent	window;
            T_inputMouse	mouse;
            T_inputKeyboard	keyboard;


        protected:

            //method

            //few protected metho to catch event for special component
            void pollEventWindow    (SDL_Event* event, Uint32 windowID);
            void pollEventMouse     (SDL_Event* event);
            void pollEventKeyboard  (SDL_Event* event);

        private:
    };
}

#endif //_GE_INPUT_MANAGER_H