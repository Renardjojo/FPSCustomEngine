#include "Game/demo.hpp"
#include "GE/Core/Parsers/objParser.hpp"
#include "GE/LowRenderer/camera.hpp"
#include "GE/LowRenderer/Light/directionnalLight.hpp"
#include "GE/LowRenderer/Light/pointLight.hpp"
#include "GE/LowRenderer/Light/spotLight.hpp"
#include "GE/Ressources/text.hpp"
#include "GE/LowRenderer/billBoard.hpp"
#include "GE/Ressources/type.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/System/TimeSystem.hpp"

#include <SDL2/SDL_mouse.h>
#include "glad/glad.h"

using namespace Game;
using namespace Engine::Ressources;
using namespace Engine::Core::Parsers;
using namespace Engine::Core::Maths;
using namespace Engine::Core::DataStructure;
using namespace Engine::LowRenderer;
using namespace Engine::Physics;
using namespace Engine::Core::Time;


Demo::Demo(Engine::GE& gameEngine)
    :   gameEngine_         (gameEngine),
        scene_              (),
        flagleftClicIsDown  (false),
        flagF1IsDown        (false),
        mouseForPlayer1     (true),
        dirCamera          {0.f, 0.f, -1.f}
{
    loadRessourceAndScene();

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
    updateControl (gameEngine_.inputManager_);       

    PhysicSystem::update();

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


void Demo::loadRessourceAndScene  ()
{
    loadGeneralRessource   (gameEngine_.ressourceManager_);

    //Load Camera
    Camera  camP1Arg ({0.f, 0.f, 15.f},
                    {0.f, 0.f, 0.f}, 
                    gameEngine_.getWinSize().width / static_cast<float>(gameEngine_.getWinSize().heigth), 0.1f, 10000.0f, 45.0f, "MainCamera");

    mainCamera = &scene_.add<Camera>(scene_.getWorld(), camP1Arg);
}

void Demo::loadGeneralRessource   (Ressources& ressourceManager)
{
    MaterialAndTextureCreateArg matDefault;
    matDefault.name_                = "DefaultMaterial";
    matDefault.pathDiffuseTexture   = nullptr;
    matDefault.flipTexture          = false;

    ModelCreateArg cube1arg     {{0.f, -5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {5.f, 0.1f, 5.f}, 
                                &ressourceManager.add<Shader>("White", "./ressources/shader/vLightObj.vs", "./ressources/shader/fLightObj.fs", AMBIANTE_COLOR_ONLY), 
                                {&ressourceManager.add<Material>("DefaultMaterial", matDefault)}, 
                                &ressourceManager.add<Mesh>("cube1", Mesh::createCube(1)),
                                "cube1"};

    scene_.add<Model>(scene_.getWorld(), cube1arg);

    matDefault.name_                = "PinkMaterial";
    matDefault.comp_.ambient.rgbi   = Vec4{1.f, 0.f, 1.f, 1.f};

    ModelCreateArg sphere1arg   {{0.f, 5.f, 0.f}, 
                                {0.f, 0.f, 0.f}, 
                                {0.5f, 0.5f, 0.5f}, 
                                &ressourceManager.get<Shader>("White"), 
                                {&ressourceManager.add<Material>(matDefault.name_, matDefault)},  
                                &ressourceManager.add<Mesh>("sphere1", Mesh::createSphere(25,25)),
                                "sphere1"};

    GameObject& sphere = scene_.add<Model>(scene_.getWorld(), sphere1arg);

    /*life bar*/             
        GameObject& lifeBar = scene_.add<Engine::LowRenderer::Entity>(sphere);       
        lifeBar.entity->setTranslation({0.f, 1.f, 0.f});
        lifeBar.entity->setName("lifeBar");

        matDefault.name_                = "BlackMaterial";
        matDefault.comp_.ambient.rgbi   = Vec4{0.f, 0.f, 0.f, 1.f};
        ModelCreateArg lifeBarBackGroundArg     {{0.f, 0.f, 0.f}, 
                                                {0.f, 0.f, 0.f}, 
                                                {1.f, 0.1f, 0.2f}, 
                                                &ressourceManager.get<Shader>("White"), 
                                                {&ressourceManager.add<Material>(matDefault.name_, matDefault)}, 
                                                &ressourceManager.add<Mesh>("Plane1", Mesh::createPlane()),
                                                "lifeBarBG"};
        scene_.add<BillBoard>(lifeBar, lifeBarBackGroundArg);

        matDefault.name_                = "GreenMaterial";
        matDefault.comp_.ambient.rgbi   = Vec4{0.f, 1.f, 0.f, 1.f};

        ModelCreateArg lifeBarInternalArg   {{0.f, -0.05f, 0.1f}, 
                                            {0.f, 0.f, 0.f}, 
                                            {0.8f, 0.1f, 0.f}, 
                                            &ressourceManager.get<Shader>("White"), 
                                            {&ressourceManager.add<Material>(matDefault.name_, matDefault)}, 
                                            &ressourceManager.get<Mesh>("Plane1"),
                                            "lifeBarBGIndicator"};
        scene_.add<BillBoard>(lifeBar, lifeBarInternalArg);

    sphere.addComponent<PhysicalObject>();
    sphere.getComponent<PhysicalObject>()->SetMass(10);
    sphere.addComponent<SphereCollider>();
    sphere.getComponent<SphereCollider>()->SetBounciness(0.5f);
    scene_.getGameObject("world/cube1").addComponent<OrientedBoxCollider>();
}

void Demo::loadLights      (Ressources& ressourceManager)
{
    ModelCreateArg lightArg     {{0.f, 0.f, 0.f},
                                {0.f, 0.f, 0.f},
                                {1.f, 1.f, 1.f},
                                &ressourceManager.get<Shader>("White"),
                                {&ressourceManager.get<Material>("DefaultMaterial")},
                                &ressourceManager.get<Mesh>("Sphere"),
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

    GameObject& pl1 = scene_.add<PointLight>(scene_.getWorld(), lightArg5);

    scene_.add<Model>(pl1, lightArg);

    static_cast<DirectionnalLight*>(sunLight->entity.get())->enable(true);
    static_cast<PointLight*>(pl1.entity.get())->enable(true);
}

void Demo::updateControl(const Engine::Core::InputSystem::Input& input)
{
    if (input.keyboard.isDown[SDL_SCANCODE_UP])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_DOWN])
    {
        Vec3 vec = dirCamera * (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(-vec);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_LEFT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(-playerDirOrtho);
    }

    if (input.keyboard.isDown[SDL_SCANCODE_RIGHT])
    {
        Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
        vecDirPlayer.rotate(-input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI).rotated90();
        Vec3 playerDirOrtho {vecDirPlayer.x, dirCamera.y, vecDirPlayer.y};

        playerDirOrtho *= (20.f * TimeSystem::getDeltaTime());
        static_cast<Camera*>(mainCamera->entity.get())->translate(playerDirOrtho);
    }

    if (input.mouse.motion.y != 0)
    {
        if (mouseForPlayer1)
        {
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.y * 0.01f * TimeSystem::getDeltaTime(), {1.f, 0.f, 0.f});
        }
    }

    if (input.mouse.motion.x != 0)
    {
        if (mouseForPlayer1)
        {
            Vec2 vecDirPlayer = {dirCamera.x, dirCamera.z};
            vecDirPlayer.rotate(input.mouse.motion.x * 0.1f * TimeSystem::getDeltaTime() * 180 / M_PI);
            dirCamera.x = vecDirPlayer.x;
            dirCamera.z = vecDirPlayer.y;
            static_cast<Camera*>(mainCamera->entity.get())->rotate(-input.mouse.motion.x * 0.01f * TimeSystem::getDeltaTime(), {0.f, 1.f, 0.f});
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