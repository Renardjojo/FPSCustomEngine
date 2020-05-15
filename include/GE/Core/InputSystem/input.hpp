//Project : rasterizer
//Editing by Six Jonathan
//Date : 21/11/2019 - 09 h 59

//function based on SDL2 library. See documentation on : https://wiki.libsdl.org/SDL_Scancode to use SDL_Scancode for keyboard
//The rest of the documentation of input : https://wiki.libsdl.org/SDL_Event

#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <SDL2/SDL_events.h>
#include <iostream>

namespace Engine::Core::InputSystem
{
    struct inputMouse
    {

        SDL_Point position{0, 0};
        SDL_Point motion{0, 0};
        SDL_Point wheel_value{0, 0};
        int wheel_scrolling{1};
        bool wheelClic{false};
        bool isTouch{false};
        bool rightClic_down{false};
        bool doubleLeftClic{false};
        bool leftClic_down{false};
        bool leftClicPressed{false};
        unsigned int clicks{0};
        float timeFirstLeftClic{0.f};

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
            {
                leftClicPressed = false;
                return 0;
            }
        }
    };
    struct inputKeyboard
    {
        SDL_Keycode key;
        SDL_Scancode up{SDL_SCANCODE_UP};
        SDL_Scancode down{SDL_SCANCODE_DOWN};
        SDL_Scancode right{SDL_SCANCODE_RIGHT};
        SDL_Scancode left{SDL_SCANCODE_LEFT};
        SDL_Scancode jump{SDL_SCANCODE_SPACE};

        bool isTouch{false};
        bool flagEscIsRelease{false};
        bool escIsRelease{false};
        bool isDown[SDL_NUM_SCANCODES]; //use Scancode enum in file SDL_Scancode.
        bool isPressed[SDL_NUM_SCANCODES];

        int onePressed(const SDL_Scancode key)
        {
            if (isDown[key] && !isPressed[key])
            {
                isPressed[key] = true;
                return 1; //Is pressed
            }
            else if (isDown[key] && isPressed[key])
            {
                isPressed[key] = true;
                return 2; //isDown
            }
            else if (!isDown[key])
            {
                isPressed[key] = false;
                return 0; //not press
            }
            else
                return 0; //not press
        }
    };
    struct windowEvent
    {
        bool windowEventHappend{false}; //true if event appendz
        bool isShow{true};              //if window is show or hidden
        bool isExposed{false};          //window has been exposed and should be redrawn
        bool isMoved{false};            //window has been moved.  Note : use SDL_GetWindowPosition to know them new data
        bool isResized{false};          //window has been resized. Note : use SDL_GetWindowSize to know them new data
        bool isMinimized{false};        // window has been minimized. Note use SDL_MinimizeWindow
        bool isMaximized{false};        // window has been maximized. Note use SDL_MaximizeWindow
        bool focusInWindow{true};       //true while mouse or keyboard focus is in focus
    };

    class Input
    {

    protected:
        //few protected metho to catch event for special component
        static void pollEventWindow(SDL_Event *event, Uint32 windowID);
        static void pollEventMouse(SDL_Event *event);
        static void pollEventKeyboard(SDL_Event *event);

    public:
        static windowEvent window;
        static inputMouse mouse;
        static inputKeyboard keyboard;

        Input();
        Input(const Input &other) = delete;
        virtual ~Input() = default;

        static void pollEvent(Uint32 windowID);
        static SDL_Scancode waitForKey();
        static void resetKeyDown();
    };
} // namespace Engine::Core::InputSystem

#endif // __INPUT_HPP__