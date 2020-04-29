#include "demo.hpp"
#include "objParser.hpp"
#include "camera.hpp"
#include "directionnalLight.hpp"
#include "pointLight.hpp"
#include "spotLight.hpp"
#include "text.hpp"
#include "billBoard.hpp"
#include "type.hpp"

#include <SDL2/SDL_mouse.h>
#include "glad.h"

using namespace Game;
using namespace Engine::Resources;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::Maths;
using namespace Engine::Core::DataStructure;
using namespace Engine::LowRenderer;


Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        mouseForPlayer1     (true),
        dirCamera          {0.f, 0.f, -1.f}
{
    loadResourceAndScene();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    SDL_ShowCursor(false);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
}


void Demo::update     () noexcept
{
    updateControl (gameEngine_.inputManager_, gameEngine_.getDeltaTime() * 3.f);

    //scene_.getEntity("world/cube1").entity->translate({0.0f, 0.0f, -0.1f});

    scene_.update();
}

void Demo::display    () const noexcept
{
    Size sizeWin = gameEngine_.getWinSize();

    glViewport(0, 0, sizeWin.width, sizeWin.heigth);
    static_cast<Camera*>(mainCamera->entity.get())->use();
    scene_.draw();
    
    glUseProgram(0);
}


void Demo::loadResourceAndScene  ()
{
    loadGeneralRessource   (gameEngine_.resourceManager_);

    //Load Camera
    Camera  camP1Arg ({0.f, 0.f, 5.f},
                    {0.f, 0.f, 0.f}, 
                    gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera");

    mainCamera = &scene_.add<Camera>(scene_.getWorld(), camP1Arg);

}

void Demo::loadGeneralRessource   (Resources& resourceManager)
{
    MaterialAndTextureCreateArg matDefault;
    matDefault.name_                = "DefaultMaterial";
    matDefault.pathDiffuseTexture   = nullptr;
    matDefault.flipTexture          = false;

    ModelCreateArg cube1arg     {{0.f, 0.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {1.f, 1.f, 1.f}, 
                                &resourceManager.add<Shader>("White", "./resources/shader/vLightObj.vs", "./resources/shader/fLightObj.fs"), 
                                {&resourceManager.add<Material>("DefaultMaterial", matDefault)}, 
                                &resourceManager.add<Mesh>("cube1", Mesh::createCube(1)),
                                "cube1"};

    scene_.add<Model>(scene_.getWorld(), cube1arg);

    ModelCreateArg sphere1arg   {{0.f, 5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {0.5f, 0.5f, 0.5f}, 
                                &resourceManager.get<Shader>("White"), 
                                {&resourceManager.get<Material>("DefaultMaterial")}, 
                                &resourceManager.add<Mesh>("sphere1", Mesh::createSphere(25,25)),
                                "sphere1"};

    scene_.add<Model>(scene_.getWorld(), sphere1arg);
}



void Demo::loadLights      (Resources& resourceManager)
{
    ModelCreateArg lightArg     {{0.f, 0.f, 0.f},
                                {0.f, 0.f, 0.f},
                                {1.f, 1.f, 1.f},
                                &resourceManager.get<Shader>("White"),
                                {&resourceManager.get<Material>("DefaultMaterial")},
                                &resourceManager.get<Mesh>("Sphere"),
                                "Ground",
                                true};

    DirectionnalLightCreateArg lightArg2 {   {0.f, 1.f, 1.f},
                                        {1.f, 1.f, 1.f, 0.1f},
                                        {1.f, 1.f, 1.f, 0.7f},
                                        {1.f, 1.f, 1.f, 1.f}, "light"};

    PointLightCreateArg lightArg5 { {50.f, 20.f, -50.f},
                                    {1.f, 1.f, 1.f, 0.f},
                                    {0.f, 1.f, 0.f, 0.7f},
                                    {1.f, 1.f, 1.f, 0.3f},
                                    0.f, 0.05f, 0.f, "light"};

    sunLight = &scene_.add<DirectionnalLight>(scene_.getWorld(), lightArg2);

    GraphEntity& pl1 = scene_.add<PointLight>(scene_.getWorld(), lightArg5);

    scene_.add<Model>(pl1, lightArg);

    static_cast<DirectionnalLight*>(sunLight->entity.get())->enable(true);
    static_cast<PointLight*>(pl1.entity.get())->enable(true);
}



void Demo::updateControl          (const Engine::Core::InputSystem::Input& input, float detlaTime)
{
    if (input.keyboard.isDown[SDL_SCANCODE_UP])
    {
        Vec3 vec = dirCamera * (20.f * detlaTime);
        static_cast<Camera*>(mainCamera->entity.get())->translate(vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_DOWN])
    {
        Vec3 vec = dirCamera * (20.f * detlaTime);
        static_cast<Camera*>(mainCamera->entity.get())->translate(-vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_LEFT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * detlaTime * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * detlaTime);
        static_cast<Camera*>(mainCamera->entity.get())->translate(-playerDirOrtho);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_RIGHT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * detlaTime * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * detlaTime);
        static_cast<Camera*>(mainCamera->entity.get())->translate(playerDirOrtho);
    }

    if (input.mouse.motion.y != 0)
    {
        if (mouseForPlayer1)
        {
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.y * 0.1f * detlaTime, {1.f, 0.f, 0.f});
        }
    }

    if (input.mouse.motion.x != 0)
    {
        if (mouseForPlayer1)
        {
            Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
            vecDirPlayer.rotate(input.mouse.motion.x * 0.1f * detlaTime * 180 / M_PI);
            dirCamera.x = vecDirPlayer.x;
            dirCamera.z = vecDirPlayer.y;
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.x * 0.1f * detlaTime, {0.f, 1.f, 0.f});
        }
    }

    static int exFrameWheelVal = input.mouse.wheel_scrolling;

    if (input.mouse.wheel_scrolling != exFrameWheelVal)
    {
        if(input.mouse.wheel_scrolling > exFrameWheelVal)
        {
            static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY + 5);
        }
        else
        {
            static_cast<Camera*>(mainCamera->entity.get())->setFovY(static_cast<Camera*>(mainCamera->entity.get())->getProjectionInfo().fovY - 5);
        }
        
        exFrameWheelVal = input.mouse.wheel_scrolling;
    }

    if (input.keyboard.isDown[SDL_SCANCODE_F1] && !flagF1IsDown)
    {
        mouseForPlayer1 = !mouseForPlayer1;
        flagF1IsDown = true;
    }
    else
    {
        flagF1IsDown = input.keyboard.isDown[SDL_SCANCODE_F1];
    }    
}