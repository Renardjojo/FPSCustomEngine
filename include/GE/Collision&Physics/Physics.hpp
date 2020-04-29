//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 10 h 53

#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include "GE/Core/Maths/vec.hpp"
#include "GE/Collision&Physics/PhysicalObject.hpp"

namespace Engine::Physics
{
    class Physic
    {
        public:

        #pragma region constructor/destructor

        Physic ()					        = delete;
        Physic (const Physic& other)		= delete;
        Physic (Physic&& other)				= delete;
        virtual ~Physic ()				    = delete;
        Physic& operator=(Physic const&)	= delete;
        Physic& operator=(Physic &&)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region methods

        static
        void update ();

        #pragma endregion //!methods

        #pragma region accessor
        #pragma endregion //!accessor

        #pragma region mutator

        /**
         * @brief Add physicalObject (using key word this) on the physic system. This object will be update by the physic system
         * 
         * @param pPhysicalObject 
         */
        static
        void addPhysicalObject (PhysicalObject* pPhysicalObject) 
        {
            pPhysicalObjects.push_back(pPhysicalObject);
        }

        #pragma endregion //!mutator

        #pragma region operator
        #pragma endregion //!operator

        #pragma region convertor
        #pragma endregion //!convertor

        protected:

        #pragma region attribut

        static Engine::Core::Maths::Vec3 gravity;

        static std::vector<PhysicalObject*> pPhysicalObjects;

        #pragma endregion //!attribut

        #pragma region static attribut
        #pragma endregion //! static attribut

        #pragma region methods
        #pragma endregion //!methods

        private:

    };
}

#endif //_PHYSICS_H