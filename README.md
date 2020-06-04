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

Vous êtes seul dans une plaine désertique à défendre le coeur de ce lieu, un cristale aussi appellé nexus. Des vagues d'ennemies essayerons de vous envahir afin de récuperer les essences de ce cristale. A vous de défendre ce lieu de jours comme de nuit ! Qui sais ce qu'il pourrait arriver si ce cristalle venait à mourir... 

![Screen shoot Engine 1](screens/ScreenShootMD1.png "Screen shoot Engine example 1")
![Screen shoot Engine 2](screens/ScreenShootMD2.png "Screen shoot Engine example 2")
![Screen shoot Engine 3](screens/ScreenShootMD3.png "Screen shoot Engine example 3")

## Commandes

### Clavier :
Les commandes sont réglabes via le menu option du jeu. Par default :

Commande 		| Action
------------- 	| -------------
Q/Z/S/D         | Déplacer le joueur : gauche/haut/bas/droite
Espace        	| Sauter / Monter en camera éditeur
Clic gauche     | Tirer
Clic droit      | Viser (pour le sniper)
molette         | Changer d'arme
R               | Recharger
F               | Utiliser la machine à butin
Esc             | Pause/Quitter
F1 				| Utiliser l'éditeur du moteur (graphe de scene, inspector, camera éditeur...)
F2 				| Switch 3eme personne/1er personne
ctrl            | Descendre en camera éditeur

## Comment compiler
Ouvrez un terminal et taper la commande suivante :

```
make
```
Vous pouvez également compiler en multi thread via la commende
```
make multi
```
D'autre commandes sont disponibles dans le fichier makefile

Les librairies suivantes sont nécéssaire :

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

## Comment executer
Ouvrez un terminal à la racine du projet et tapez :
```
make run
ou
./bin/exe
```
/!\ Attention vous devez utiliser un terminal indépendant et non celui d'un IDE sinon, le son ne fonctionnera pas.

Il est possible de compiler le programme avec différente flag en modifier directement le makefile :
- Pour la version debug avec editeur : CXX_DEBUG en c++ et C_DEBUG en C
- Pour la version release sans editeur : CXX_EDITOR en c++ et C_EDITOR en C
- Pour la version release avec editeur : CXX_BUILD en c++ et C_BUILD en C

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

Moteur physique réalisé grâce à nos propres libraires de mathématique. Ce moteur est réliasé en suivant l'architecture ECS (Entity-Components-System) et reprend certain interface que le moteur unity nous a inspiré.

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
- Sauvegarde des prefabs et des scene en XML
