# Projet FPS avec moteur de jeu OpenGl3.3/SDL2.0
> Gavelle Anthony, Nisi Guillaume, Six Jonathan
> Juin 2020

## Summary
- [Description](##Description "Goto description part")
- [Commandes](##Commandes "Goto command part")
- [Comment compiler](##Comment-compiler "Goto compil part")
- [Comment éxécuter](##Comment-executer "Goto launch part")
- [Bug](##Bug "Goto bug part")
- [Aspect technique](##Aspect-technique "Goto technical part")

## Description

Vous êtes seule dans une plaine désertique à défendre le coeur de ce lieu, un cristale aussi appellé nexus. Des vagues d'ennemies esseyrons de vous envahir afin de récuperer les essences de ce cristale. A vous de défendre se lieu de jours comme de nuit ! Qui sais se qu'il pourrait arriverer si ce cristalle venait à mourir... 

![Screen shoot Engine 1](screens/ScreenShootMD1.png "Screen shoot Engine example 1")
![Screen shoot Engine 2](screens/ScreenShootMD2.png "Screen shoot Engine example 2")
![Screen shoot Engine 3](screens/ScreenShootMD3.png "Screen shoot Engine example 3")

## Commandes

### Clavier :
Les commandes sont réglave via le menu option du jeu. Par default :

Commande 		| Action
------------- 	| -------------
Q/Z/S/D         | Déplacer le joueur : gauche/haut/bas/droite
Espace        	| Sauter / Monter en camera editeur
Clic gauche     | Tirer
Clic droit      | viser (pour le sniper)
molette         | changer d'arme
R               | recharger
F               | utiliser la machine à butin
Esc             | Pause/Quitter
F1 				| utiliser l'éditeur du moteur (graphe de scene, inspector, camera libre...)
F2 				| Switch 3eme personne/1er personne
ctrl            | descendre en camera editeur

## Comment compiler
Ouvrez un terminal et taper la commande suivante :

```
make
```
Vous pouvez également compiler en multi thread via la comment
```
make multi
```
D'autre commande sont disponible dans le fichier makefile

## Comment executer
Ouvrez un terminal à la racine du projet et tapez :
```
make run
ou
./bin/exe
```
/!\ Attention vous devez utiliser un terminal indépendant et non celui d'un IDE. Sinon le son ne fonctionnera pas.

## Bug

Tous cas de collision n'ont pus être testé. La version actuelle est stable mais il peut y avoir certain cas particulier non détecté lors des playtests.

## Aspect technique

### Leak : 

La SDL leak à hauteur de 

### Performance :

Ce projet tourne entre 30 et 140 FPS en moyenne.

### Autre :

Projet réaliser grâce à un moteur fonctionnant grâce au libraire suivante :
- OpenGL en core profile (<3.3)
- SDL2.0
- ImGui
- STB_image
- OpenAl

Moteur physique réalisé grâce à nos propres libraires de mathématique. Ce moteur est réliaser en suivant l'architecture ECS (Entity-Components-System) et reprend certain interface que le moteur unity nous a inspiré.

- time manager (possède un system de fixed update et unfixed update pour les calcule de physique. Possibilité d'utiliser un time scale pour accélerer ou ralentir le temps)
- input manager
- Variadique template ressource manager
- Systeme de son (Utilise OpenAL et ne lis que les fichier WAV en raison de sa license)
- Parser de MTL et fichier OBJ (ne retourne qu'une seule mesh)
- Fichier de log si le dossier "log" est crée
- Systeme de transform, gameObject et component
- Billboard (entité toujours face à la caméra)
- Ecran splitté possible et déjà réalisé
- Textes (avec SDL_TTF)
- Skybox
- Pré-construction des shaders avec tris des informations envoyé en fonction
- multi texturing et multi matériaux pour les meshs
- Lumière fonctionnant avec l'agorythm Blinn-Phong
- Générateur de particule et de décalle

If Lib is missing (undefine reference or include probleme)
Please check if this lib is install :

lib OpenGL : 
- sudo apt-get install libglew-dev
- sudo apt-get install glew-utils

C, C++ compilor
- sudo apt-get install gcc
- sudo apt-get install g++

lib SDL2
- sudo apt-get install libsdl2-dev
- sudo apt-get install libsdl2-ttf-dev
- sudo apt-get install libsdl2-image-dev

lib OpenAl :
- sudo apt-get install libopenal-dev
- sudo apt-get install libalut-dev
