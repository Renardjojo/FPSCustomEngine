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

        /*Shot sound*/
        Engine::Ressources::Sound*  _sound      {nullptr};

        public:

        Firearm(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float reloadTime, unsigned int munitionCapacity, float shotIntervalDelay, bool isAutomatic,Engine::Ressources::Sound* sound)
            :   Engine::Core::Component::ScriptComponent    {gameObject},        
                _bulletDamage                               {bulletDamage},
                _bulletVelocity                             {bulletVelocity},
                _bulletPerShot                              {bulletPerShot},
                _reloadTime                                 {reloadTime},
                _munitionCapacity                           {munitionCapacity},
                _shotIntervalDelay                          {shotIntervalDelay},
                _munition                                   {munitionCapacity},
                _delay                                      {0.f},
                _isReloading                                {false},
                _isWaitingForNextShot                       {false},
                _isAutomatic                                {isAutomatic},
                _sound                                      {sound}
        {
            _name = __FUNCTION__;
            GE_assertInfo(_munitionCapacity % _bulletPerShot == 0, "The magazine must match the number of bullets fired per shot");
            GE_assert(reloadTime > 0.f);
            GE_assert(shotIntervalDelay > 0.f);
        }

        Firearm (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _bulletDamage                               {std::stof(params[0])},
                _bulletVelocity                             {std::stof(params[1])},
                _bulletPerShot                              {static_cast<unsigned int>(std::stoi(params[2]))},
                _reloadTime                                 {std::stof(params[3])},
                _munitionCapacity                           {static_cast<unsigned int>(std::stoi(params[4]))},
                _shotIntervalDelay                          {std::stof(params[5])},
                _munition                                   {static_cast<unsigned int>(std::stoi(params[6]))},
                _delay                                      {std::stof(params[7])},
                _isReloading                                {static_cast<bool>(std::stoi(params[8]))},
                _isWaitingForNextShot                       {static_cast<bool>(std::stoi(params[9]))},
                _isAutomatic                                {static_cast<bool>(std::stoi(params[10]))},
                _sound                                      {nullptr}
        {
            _name = __FUNCTION__;
            GE_assertInfo(_munitionCapacity % _bulletPerShot == 0, "The magazine must match the number of bullets fired per shot");
            GE_assert(_reloadTime > 0.f);
            GE_assert(_shotIntervalDelay > 0.f);
        }

        virtual ~Firearm() = default;

        void update () noexcept
        {
            if (_isReloading)
            {
                _delay += Engine::Core::System::TimeSystem::getDeltaTime();

                if (_delay >= _reloadTime)
                {
                    _isReloading = false;
                    _munition = _munitionCapacity;
                    _delay = 0.f;
                }
            }
            else if (_isWaitingForNextShot)
            {
                _delay += Engine::Core::System::TimeSystem::getDeltaTime();

                if (_delay >= _shotIntervalDelay)
                {
                    _isWaitingForNextShot = false;
                    _delay = 0.f;
                }
            }
        }

        virtual bool isAutomatic() const noexcept { return _isAutomatic;}

        virtual void shoot (const Engine::Core::Maths::Vec3& startPoint, const Engine::Core::Maths::Vec3& direction) noexcept
        {
            if (_isReloading || _isWaitingForNextShot)
            {
                return;
            }

            /*Auto reload*/
            if (_munition == 0)
            {
                reload();
                return;
            }

            if(_sound)
                Engine::Ressources::SoundPlayer::play(*_sound);

            _munition -= _bulletPerShot;
            _isWaitingForNextShot = true;
            
            Engine::Physics::ColliderShape::HitInfo rayInfo;

            for (size_t i = 0; i < _bulletPerShot; i++)
            {
                //TODO: remove direction * 6.f when layer integrate the player (avoid to shoot the player)
                if (Engine::Physics::PhysicSystem::rayCast(startPoint + direction * 6.f, direction, 10000.f, rayInfo))
                {
                    Engine::Physics::ColliderShape::Collider* pCollider = rayInfo.gameObject->getComponent<Engine::Physics::ColliderShape::Collider>();
                    Engine::Ressources::GameObject tempGOWithTag;
                    tempGOWithTag.setTag("Bullet");
                    Engine::Physics::ColliderShape::HitInfo hitInfo1 {rayInfo.intersectionsInfo, &tempGOWithTag, _bulletVelocity};
                    pCollider->OnCollisionEnter(hitInfo1);

                    if (rayInfo.gameObject->getTag() != "Ground")
                        rayInfo.gameObject->destroy();
                }
            }
        }

        virtual void reload () noexcept
        {
            _isReloading = true;
        }

        virtual void aim () noexcept
        {

        }

        unsigned int getMunition () const noexcept
        {
            return _munition;
        }

        unsigned int* getPMunition () noexcept
        {
            return &_munition;
        }

        unsigned int getMagazine () const noexcept
        {
            return _munitionCapacity;
        }

        virtual void save(xml_document<>& doc, xml_node<>* nodeParent) 
        {
            xml_node<> *newNode = doc.allocate_node(node_element, "COMPONENT");

            newNode->append_attribute(doc.allocate_attribute("type", _name.c_str()));
            newNode->append_attribute(doc.allocate_attribute("bulletDamage", doc.allocate_string(std::to_string(_bulletDamage).c_str())));
            newNode->append_attribute(doc.allocate_attribute("bulletVelocity", doc.allocate_string(std::to_string(_bulletVelocity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("bulletPerShot", doc.allocate_string(std::to_string(_bulletPerShot).c_str())));
            newNode->append_attribute(doc.allocate_attribute("reloadTime", doc.allocate_string(std::to_string(_reloadTime).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munitionCapacity", doc.allocate_string(std::to_string(_munitionCapacity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("shotIntervalDelay", doc.allocate_string(std::to_string(_shotIntervalDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munition", doc.allocate_string(std::to_string(_munition).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isReloading", doc.allocate_string(std::to_string(_isReloading).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isWaitingForNextShot", doc.allocate_string(std::to_string(_isWaitingForNextShot).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isAutomatic", doc.allocate_string(std::to_string(_isAutomatic).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_FIRE_ARM_H