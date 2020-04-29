//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef _PHYSICAL_OBJECT_H
#define _PHYSICAL_OBJECT_H

#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/DataStructure/graph.hpp"
#include "GE/Ressources/Component.hpp"

using namespace Engine::Core::Maths;

namespace Engine::Physics
{
    class PhysicalObject : public Engine::Ressources::Component
    {
        public:

        #pragma region constructor/destructor

        PhysicalObject (Engine::Ressources::GameObject& refGameObject);
        PhysicalObject (const PhysicalObject& other);
        PhysicalObject (PhysicalObject&& other);
        virtual ~PhysicalObject ();
        
        PhysicalObject ()                                       = delete;
        PhysicalObject& operator=(PhysicalObject const& other)  = delete;
        PhysicalObject& operator=(PhysicalObject && other)      = delete;

        #pragma endregion //!constructor/destructor

        #pragma region methods

        void AddForce(Vec3 force);
        void AddForce(float x, float y, float z);
        void AddTorque(Vec3 force);
        void AddTorque(float x, float y, float z);

        #pragma endregion //!methods

        #pragma region accessor

        Vec3 GetVelocity()          { return velocity; }
        Vec3 GetAngularVelocity()   { return angularVelocity; }
        float GetMass()             { return mass; }

        bool IsKinematic() { return _isKinematic; }
        bool UseGravity() { return _useGravity; }
        Engine::Ressources::GameObject& getGameObject () { return gameObject;}

        #pragma endregion //!accessor

        #pragma region mutator

        void SetVelocity(Vec3 _velocity) { velocity = _velocity; }
        void SetVelocity(float x, float y, float z) { velocity.x = x; velocity.y = y; velocity.z = z;}  
        void SetMass(float _mass) { mass = _mass; } 
        void SetIsKinematic(bool state) { _isKinematic = state; }
        void SetUseGravity(bool state) { _useGravity = state; }
        void SetFreezeTrX(bool state) { freezeTrX = state; }
        void SetFreezeTrY(bool state) { freezeTrY = state; }
        void SetFreezeTrZ(bool state) { freezeTrZ = state; }
        void SetFreezeRotX(bool state) { freezeRotX = state; }
        void SetFreezeRotY(bool state) { freezeRotY = state; }
        void SetFreezeRotZ(bool state) { freezeRotZ = state; }

        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor


        protected:

        #pragma region attribut

        Vec3 velocity = {0,0,0};
        Vec3 angularVelocity;
        float mass = 1;
        bool freezeTrX = false;
        bool freezeTrY = false;
        bool freezeTrZ = false;
        bool freezeRotX = false;
        bool freezeRotY = false;
        bool freezeRotZ = false;
        bool _isKinematic = false;
        bool _useGravity = true;

        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        private:

    };
}

#endif //_PHYSICAL_OBJECT_H