#include "GE/Core/InputSystem/input.hpp"

#ifndef DNEDITOR
#include "imgui/imgui_impl_sdl.h"
#endif

using namespace Engine::Core::InputSystem;

windowEvent Input::window;
inputMouse Input::mouse;
inputKeyboard Input::keyboard;

int inputKeyboard::onePressed(const SDL_Scancode key)
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

Input::Input()
{
	for (unsigned int index = 0; index < (unsigned int)SDL_NUM_SCANCODES; index++)
		keyboard.isDown[index] = false;
}

void Input::pollEvent(Uint32 windowID)
{
	SDL_Event event;

	//reset input
	window.windowEventHappend = false;
	mouse.isTouch = false;
	keyboard.isTouch = false;

	mouse.isTouch = false;
	window.isExposed = false;
	window.isMoved = false;
	window.isResized = false;
	window.isMinimized = false;
	window.isMaximized = false;
	keyboard.escIsRelease = false;
	mouse.motion = {0, 0};


	mouse.oldLeftClicDown = mouse.leftClicDown;
	mouse.oldRightClicDown = mouse.rightClicDown;

	while (SDL_PollEvent(&event))
	{
#ifndef DNEDITOR
		ImGui_ImplSDL2_ProcessEvent(&event);
#endif

		switch (event.type)
		{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
			mouse.isTouch = true;
			pollEventMouse(&event);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			keyboard.isTouch = true;
			pollEventKeyboard(&event);
			break;

		case SDL_WINDOWEVENT:
			window.windowEventHappend = true;
			pollEventWindow(&event, windowID);
			break;

		case SDL_QUIT:
			exit(EXIT_SUCCESS);
			break;

		default:
			break;
		}
	}

	// std::cout << "old :" << mouse.oldLeftClicDown << " New: " << mouse.leftClicDown << std::endl;

	if (!mouse.oldLeftClicDown && mouse.leftClicDown)
		mouse.leftClicDownOnce = true;
	else
		mouse.leftClicDownOnce = false;

	if (!mouse.oldRightClicDown && mouse.rightClicDown)
		mouse.rightClicDownOnce = true;
	else
		mouse.rightClicDownOnce = false;
}

void Input::pollEventWindow(SDL_Event *event, Uint32 windowID)
{
	if (event == NULL || event->window.windowID != windowID)
		return;

	switch (event->window.event)
	{
	case SDL_WINDOWEVENT_SHOWN:
		window.isShow = true;
		break;

	case SDL_WINDOWEVENT_HIDDEN:
		window.isShow = false;
		break;

	case SDL_WINDOWEVENT_EXPOSED:
		window.isExposed = true;
		break;

	case SDL_WINDOWEVENT_MOVED:
		window.isMoved = true;
		break;

	case SDL_WINDOWEVENT_SIZE_CHANGED:
		window.isResized = true;
		break;

	case SDL_WINDOWEVENT_MINIMIZED:
		window.isMinimized = true;
		break;

	case SDL_WINDOWEVENT_MAXIMIZED:
		window.isMaximized = true;
		break;

	case SDL_WINDOWEVENT_ENTER:
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		window.focusInWindow = true;
		break;

	case SDL_WINDOWEVENT_LEAVE:
	case SDL_WINDOWEVENT_FOCUS_LOST:
		window.focusInWindow = false;
		break;

	case SDL_WINDOWEVENT_CLOSE:
		event->type = SDL_QUIT;
		SDL_PushEvent(event);
		break;

	default:
		break;
	}
}

void Input::pollEventMouse(SDL_Event *event)
{
	if (event == NULL)
		return;


	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button)
		{
		case SDL_BUTTON_MIDDLE:
			mouse.wheelClic = false;
			break;

		case SDL_BUTTON_RIGHT:
			mouse.rightClicDown = true;
			break;

		case SDL_BUTTON_LEFT:
			mouse.leftClicDown = true;
			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		switch (event->button.button)
		{
		case SDL_BUTTON_MIDDLE:
			mouse.wheelClic = false;
			break;

		case SDL_BUTTON_RIGHT:
			mouse.rightClicDown = false;
			break;

		case SDL_BUTTON_LEFT:
			mouse.leftClicDown = false;

			if (event->button.timestamp * 0.001 - mouse.timeFirstLeftClic < 0.2)
				mouse.doubleLeftClic = true;
			else
				mouse.doubleLeftClic = false;

			mouse.timeFirstLeftClic = event->button.timestamp * 0.001;
			break;
		}

		break;

	case SDL_MOUSEMOTION:
		mouse.position.x = event->button.x;
		mouse.position.y = event->button.y;
		mouse.motion.x = event->motion.xrel;
		mouse.motion.y = event->motion.yrel;
		break;

	case SDL_MOUSEWHEEL:
		mouse.wheel_value.x = event->wheel.x;
		mouse.wheel_value.y = event->wheel.y;

		mouse.wheel_scrolling += event->wheel.y;
		break;
	}
}

void Input::pollEventKeyboard(SDL_Event *event)
{
	if (event == NULL)
	{
		return;
	}

	if (event->type == SDL_KEYDOWN)
		keyboard.key = event->key.keysym.sym;

	keyboard.isDown[event->key.keysym.scancode] = (event->type == SDL_KEYDOWN);

	if (!keyboard.isDown[SDL_SCANCODE_ESCAPE] && keyboard.flagEscIsRelease)
	{
		keyboard.escIsRelease = true;
		keyboard.flagEscIsRelease = false;
	}

	if (keyboard.isDown[SDL_SCANCODE_ESCAPE])
		keyboard.flagEscIsRelease = true;
}

void Input::resetKeyDown()
{
	keyboard.isDown[SDL_SCANCODE_RETURN] = false;
	//mouse.leftClicDown = false;
}

SDL_Scancode Input::waitForKey()
{
	SDL_Event event;
	while ((!keyboard.isTouch || keyboard.isDown[SDL_SCANCODE_RETURN]))
	{
		SDL_PollEvent(&event);
		if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			return SDL_SCANCODE_UNKNOWN;
		}

		if (event.type == SDL_KEYDOWN)
		{
			return event.key.keysym.scancode;
		}
	}
	return SDL_SCANCODE_UNKNOWN;
}