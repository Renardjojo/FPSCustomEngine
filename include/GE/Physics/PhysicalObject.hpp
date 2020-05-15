//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef _PHYSICAL_OBJECT_H
#define _PHYSICAL_OBJECT_H

#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/Component.hpp"

namespace Engine::Physics
{
    class PhysicalObject : public Engine::Ressources::Component
    {
    public:
#pragma region constructor / destructor

        PhysicalObject(Engine::Ressources::GameObject &refGameObject);
        PhysicalObject(const PhysicalObject &other);
        PhysicalObject(PhysicalObject &&other);
        virtual ~PhysicalObject();

        PhysicalObject() = delete;
        PhysicalObject &operator=(PhysicalObject const &other) = delete;
        PhysicalObject &operator=(PhysicalObject &&other) = delete;

#pragma endregion //!constructor/destructor

#pragma region methods

        void AddForce(Engine::Core::Maths::Vec3 force);
        void AddForce(float x, float y, float z);
        void AddTorque(Engine::Core::Maths::Vec3 force);
        void AddTorque(float x, float y, float z);

#pragma endregion //!methods

#pragma region accessor

        Engine::Core::Maths::Vec3 GetVelocity()
        {
            return velocity;
        }
        Engine::Core::Maths::Vec3 GetAngularVelocity() { return angularVelocity; }
        float GetMass() { return mass; }

        bool IsKinematic() { return _isKinematic; }
        bool UseGravity() { return _useGravity; }
        bool IsSleeping() { return _sleep; }
        Engine::Ressources::GameObject &getGameObject() { return gameObject; }

#pragma endregion //!accessor

#pragma region mutator

        void SetVelocity(Engine::Core::Maths::Vec3 _velocity)
        {
            _sleep = false;
            velocity = _velocity;
        }
        void SetVelocity(float x, float y, float z)
        {
            _sleep = false;
            velocity.x = x;
            velocity.y = y;
            velocity.z = z;
        }
        void SetMass(float _mass) { mass = _mass; }
        void SetIsKinematic(bool state) { _isKinematic = state; }
        void SetUseGravity(bool state) { _useGravity = state; }
        void SetFreezeTrX(bool state) { freezeTrX = state; }
        void SetFreezeTrY(bool state) { freezeTrY = state; }
        void SetFreezeTrZ(bool state) { freezeTrZ = state; }
        void SetFreezeRotX(bool state) { freezeRotX = state; }
        void SetFreezeRotY(bool state) { freezeRotY = state; }
        void SetFreezeRotZ(bool state) { freezeRotZ = state; }

        void Sleep() { _sleep = true; }
        void WakeUp() { _sleep = false; }


#pragma endregion //!mutator

    protected:
#pragma region attribut

        Engine::Core::Maths::Vec3 velocity{0, 0, 0};
        Engine::Core::Maths::Vec3 angularVelocity{0, 0, 0};
        float mass{1};
        bool freezeTrX{false};
        bool freezeTrY{false};
        bool freezeTrZ{false};
        bool freezeRotX{false};
        bool freezeRotY{false};
        bool freezeRotZ{false};
        bool _isKinematic{false};
        bool _useGravity{true};
        bool _sleep{false}; 

#pragma endregion //!attribut

    private:
    };
} // namespace Engine::Physics

#endif //_PHYSICAL_OBJECT_H