# Engie
> Six Jonathan.
> January 2020

## Summary
- [Description](##Description "Goto description part")
- [Command](##Command "Goto command part")
- [How to compil](##How-to-compil "Goto compil part")
- [How to launch](##How-to-launch "Goto launch part")
- [Bug](##Bug "Goto bug part")
- [Technical aspect](##Technical-aspect "Goto technical part")

## Description

This project is a game engine create with SDL2 and Opengl 3.3 with core profile.
This engine own lots of feature :
- time manager (this delta time system)
- input manager
- resources manager with variadic template and access to resources data using keys
- Sound system (sample and music managed with SDL_Mixer)
- Parser of MTL and OBJ file
- Log file with different setting (by default create log fil if error happend)
- Scene graph with all entity (camera, light, model...)
- Billboard entity (alway front off camera)
- Splitted screen with multi player
- Text (with SDL_TTF)
- Skybox
- Shader with #include system or with modul (for light and SkyBox)
- Multi textured and multi materials models

![Screen shoot Engine 1](screens/ScreenShootMD1.png "Screen shoot Engine example 1")
![Screen shoot Engine 2](screens/ScreenShootMD2.png "Screen shoot Engine example 2")
![Screen shoot Engine 3](screens/ScreenShootMD3.png "Screen shoot Engine example 3")

## Command

### Keyboard :
Command 		| Action
------------- 	| -------------
A/W/S/D         	| player1 movement : left/up/down/right
LEFT/UP/DOWN/RIGHT  | player2 movement : left/up/down/right
F1 					| Switch mouse control between the two players
ctrl				| go down player 1
space				| go up player 1

## How to compil
Open terminal in source of project and execut this command

```
make
```

## How to launch

```
make run
or
./bin/exe
```

## Bug

## Technical aspect

All SDL component isn't use. SDL leak.

Ligth run with Phong algorythm

This project is depending to :
- SDL 2.0
- STB_Image
- SDL_TTF
- SDL_Mixer
- OpenGl 3.3

If Lib is missing (undefine reference or include probleme)
Please check if this lib is install :
lib OpenGL : 
    sudo apt-get install libglew-dev
    sudo apt-get install glew-utils

C, C++ compilor
    sudo apt-get install gcc
    sudo apt-get install g++

lib SDL2
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-ttf-dev
    sudo apt-get install libsdl2-image-dev
    sudo apt-get install libsdl2-gfx-dev
    sudo apt-get install libsdl2-mixer-dev

de GLFW :
    sudo apt-get install libglfw3
    sudo apt-get install libglfw3-dbgsym
    sudo apt-get install libglfw3-dev
    sudo apt-get install libglfw3-doc

SDL does'nt support sound card on linux for few system. If you have this error : Linux: ALSA lib pcm_dmix.c:1108:(snd_pcm_dmix_open) unable to open slave
Please follow this instruction :
1 - Create the /etc/modprobe.d/default.conf file: (sudo touch default.conf)
2 - Modify this file (sudo gedit default.conf &) and add this line : options snd_hda_intel index=1
3 - reboot your system (sudo reboot) and run again the project