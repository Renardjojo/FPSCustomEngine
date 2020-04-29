//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 11 h 38

#ifndef _PHYSICAL_OBJECT_H
#define _PHYSICAL_OBJECT_H

#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/DataStructure/graph.hpp"

using namespace Engine::Core::Maths;

namespace Engine::Physics
{
    class PhysicalObject
    {
        public:

        #pragma region constructor/destructor

        PhysicalObject (Engine::Core::DataStructure::GraphEntity& refGameObject);
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

        #pragma endregion //!methods

        #pragma region accessor

        Vec3 GetVelocity()          { return velocity; }
        Vec3 GetAngularVelocity()   { return angularVelocity; }
        float GetMass()             { return mass; }

        bool isKinematic() { return _isKinematic; }
        bool useGravity() { return _useGravity; }
        Engine::Core::DataStructure::GraphEntity& getGameObject () { return gameObject;}

        #pragma endregion //!accessor

        #pragma region mutator

        void SetVelocity(Vec3 _velocity) { velocity = _velocity; }
        void SetVelocity(float x, float y, float z) { velocity.x = x; velocity.y = y; velocity.z = z;}  
        void SetMass(float _mass) { mass = _mass; } 
        void SetKinematic(bool state) { _isKinematic = state; }
        void SetGravity(bool state) { _useGravity = state; }
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

        Vec3 velocity;
        Vec3 angularVelocity;
        float mass;
        bool freezeTrX;
        bool freezeTrY;
        bool freezeTrZ;
        bool freezeRotX;
        bool freezeRotY;
        bool freezeRotZ;
        bool _isKinematic;
        bool _useGravity;

        Engine::Core::DataStructure::GraphEntity& gameObject;

        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        private:

    };
}

#endif //_PHYSICAL_OBJECT_H