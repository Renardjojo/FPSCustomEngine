#include "Game/Sniper.hpp"

using namespace Game;

Sniper::Sniper(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay,Engine::Ressources::Sound* sound)
    :   Firearm {gameObject, bulletDamage, bulletVelocity, bulletPerShot, reloadTime, munitionCapacity, shotIntervalDelay, false,sound}   
{
    _name = __FUNCTION__;
}

Sniper::Sniper (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
    :   Firearm {refGameObject, params} 
{
    _name = __FUNCTION__;
}

void Sniper::switchAimState () noexcept
{
    Firearm::switchAimState();

    if(_isAiming)
    {
        /*activate and desactivate GUI and current weapon*/
        Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Image>("SniperScope").isActive = true;
        Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Image>("CrosshairImage").isActive = false;
        _gameObject.getComponent<Engine::LowRenderer::Model>()->setActive(false);

        /*Chang camera zoom*/
        float currentFovY = Engine::LowRenderer::Camera::getCamUse()->getProjectionInfo().fovY;
        Engine::LowRenderer::Camera::getCamUse()->setFovY(currentFovY / 6.f);

        /*Chang mouse speed*/
        PlayerController* playerControllerComp = Engine::Ressources::Scene::getCurrentScene()->getGameObject("Players/Player1").getComponent<PlayerController>();
        float currentMouseSpeed = playerControllerComp->getMouseSpeed();
        playerControllerComp->setMouseSpeed(currentMouseSpeed / 12.f);
    }
    else
    {
        /*activate and desactivate GUI and current weapon*/
        Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Image>("SniperScope").isActive = false;
        Engine::Ressources::t_RessourcesManager::getRessourceManagerUse()->get<Engine::Ressources::Image>("CrosshairImage").isActive = true;
        _gameObject.getComponent<Engine::LowRenderer::Model>()->setActive(true);

        /*Chang camera zoom*/
        float currentFovY = Engine::LowRenderer::Camera::getCamUse()->getProjectionInfo().fovY;
        Engine::LowRenderer::Camera::getCamUse()->setFovY(currentFovY * 6.f);

        /*Chang mouse speed*/
        PlayerController* playerControllerComp = Engine::Ressources::Scene::getCurrentScene()->getGameObject("Players/Player1").getComponent<PlayerController>();
        float currentMouseSpeed = playerControllerComp->getMouseSpeed();
        playerControllerComp->setMouseSpeed(currentMouseSpeed * 12.f);
    }
}