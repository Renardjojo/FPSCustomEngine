//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-30 - 17 h 39

#ifndef _FIRE_GUN_H
#define _FIRE_GUN_H

#include "GE/Core/Component/ScriptComponent.hpp"
#include "GE/Core/System/TimeSystem.hpp"
#include "GE/Ressources/GameObject.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicSystem.hpp"

namespace Game
{
    class FireGun : public Engine::Core::Component::ScriptComponent
    {
        protected :

        /*Bullet stats*/
        float           _bulletDamage        {1.f};
        float           _bulletVelocity      {1.f};
        unsigned int    _bulletPerShot       {1};

        /*Fire gun stats*/
        float           _realoadTime        {1.f};      //in second
        unsigned int    _munitionCapacity   {10};
        float           _shotIntervalDelay  {10};       //in second

        /*Shot sound*/
        //TODO:

        /*Current stat*/
        unsigned int    _munition               {_munitionCapacity};
        float           _delay                  {0.f};
        bool            _isRelaoading           {false};
        bool            _isWaitingForNextShot   {false};
        bool            _isAutomatic            {false}; //on fire gun stats but her for memory alignas


        public:

        FireGun(Engine::Ressources::GameObject &gameObject, float bulletDamage, float bulletVelocity, unsigned int bulletPerShot, float realoadTime, unsigned int munitionCapacity, float shotIntervalDelay, bool isAutomatic)
            :   Engine::Core::Component::ScriptComponent    {gameObject},        
                _bulletDamage                               {bulletDamage},
                _bulletVelocity                             {bulletVelocity},
                _bulletPerShot                              {bulletPerShot},
                _realoadTime                                {realoadTime},
                _munitionCapacity                           {munitionCapacity},
                _shotIntervalDelay                          {shotIntervalDelay},
                _munition                                   {munitionCapacity},
                _delay                                      {0.f},
                _isRelaoading                               {false},
                _isWaitingForNextShot                       {false},
                _isAutomatic                                {isAutomatic}
        {
            _name = __FUNCTION__;
            GE_assertInfo(_munitionCapacity % _bulletPerShot == 0, "The magazine must match the number of bullets fired per shot");
            GE_assert(realoadTime > 0.f);
            GE_assert(shotIntervalDelay > 0.f);
        }

        FireGun (Engine::Ressources::GameObject &refGameObject, const std::vector<std::string>& params)
            :   Engine::Core::Component::ScriptComponent    {refGameObject},
                _bulletDamage                               {std::stof(params[0])},
                _bulletVelocity                             {std::stof(params[1])},
                _bulletPerShot                              {static_cast<unsigned int>(std::stoi(params[2]))},
                _realoadTime                                {std::stof(params[3])},
                _munitionCapacity                           {static_cast<unsigned int>(std::stoi(params[4]))},
                _shotIntervalDelay                          {std::stof(params[5])},
                _munition                                   {static_cast<unsigned int>(std::stoi(params[6]))},
                _delay                                      {std::stof(params[7])},
                _isRelaoading                               {static_cast<bool>(std::stoi(params[8]))},
                _isWaitingForNextShot                       {static_cast<bool>(std::stoi(params[9]))}
        {
            _name = __FUNCTION__;
            GE_assertInfo(_munitionCapacity % _bulletPerShot == 0, "The magazine must match the number of bullets fired per shot");
            GE_assert(_realoadTime > 0.f);
            GE_assert(_shotIntervalDelay > 0.f);
        }

        virtual ~FireGun() = default;

        void update () noexcept
        {
            if (_isRelaoading)
            {
                _delay += Engine::Core::System::TimeSystem::getDeltaTime();

                if (_delay >= _realoadTime)
                {
                    _isRelaoading = false;
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
            if (_isRelaoading || _isWaitingForNextShot)
            {
                return;
            }

            /*Auto reload*/
            if (_munition == 0)
            {
                reload();
                return;
            }

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
            _isRelaoading = true;
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
            newNode->append_attribute(doc.allocate_attribute("realoadTime", doc.allocate_string(std::to_string(_realoadTime).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munitionCapacity", doc.allocate_string(std::to_string(_munitionCapacity).c_str())));
            newNode->append_attribute(doc.allocate_attribute("shotIntervalDelay", doc.allocate_string(std::to_string(_shotIntervalDelay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("munition", doc.allocate_string(std::to_string(_munition).c_str())));
            newNode->append_attribute(doc.allocate_attribute("delay", doc.allocate_string(std::to_string(_delay).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isRelaoading", doc.allocate_string(std::to_string(_isRelaoading).c_str())));
            newNode->append_attribute(doc.allocate_attribute("isWaitingForNextShot", doc.allocate_string(std::to_string(_isWaitingForNextShot).c_str())));

            nodeParent->append_node(newNode);
        }
    };

} //namespace Game

#endif //_FIRE_GUN_H