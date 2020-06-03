//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _FIRE_ARM_H
#define _FIRE_ARM_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicSystem.hpp"
#include "GE/Ressources/Sound.hpp"
#include "GE/Ressources/SoundPlayer.hpp"


namespace Game
{
    class Firearm : public Engine::Core::Component::ScriptComponent
    {
        protected :

        /*Bullet stats*/
        float           _bulletDamage           {1.f};
        float           _bulletVelocity         {1.f};
        unsigned int    _bulletPerShot          {1};

        /*Fire gun stats*/
        float           _reloadTime             {1.f};      //in second
        unsigned int    _munitionCapacity       {10};
        float           _shotIntervalDelay      {10};       //in second


        /*Current stat*/
        unsigned int    _munition               {_munitionCapacity};
        float           _delay                  {0.f};
        bool            _isReloading            {false};
        bool            _isWaitingForNextShot   {false};
        bool            _isAutomatic            {false}; //on fire gun stats but her for memory alignas
        bool            _isAiming               {false};

        /*Shot sound*/
        Engine::Ressources::Sound*  _sound      {nullptr};

        public:

        Firearm(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay, bool isAutomatic,Engine::Ressources::Sound* sound);

        Firearm (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params);

        virtual ~Firearm() = default;

        void update () noexcept;

        virtual bool isAutomatic() const noexcept { return _isAutomatic;}

        virtual bool isAiming() const noexcept { return _isAiming; }

        virtual void shoot (const Engine::Core::Maths::Vec3& startPoint, const Engine::Core::Maths::Vec3& direction) noexcept;

        virtual void reload () noexcept { _isReloading = true; }

        virtual void switchAimState () noexcept {_isAiming = !_isAiming; }

        unsigned int getMunition () const noexcept { return _munition;}

        unsigned int* getPMunition () noexcept {return &_munition;}

        unsigned int getMagazine () const noexcept { return _munitionCapacity;}

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent);
    };

} //namespace Game

#endif //_FIRE_ARM_H