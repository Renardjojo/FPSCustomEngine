//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-29 - 10 h 53

#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>
#include "GE/Core/Maths/vec.hpp"
#include "GE/Physics/PhysicalObject.hpp"

namespace Engine::Physics
{
    class PhysicSystem
    {
        public:

        #pragma region constructor/destructor

        PhysicSystem ()					                = delete;
        PhysicSystem (const PhysicSystem& other)	    = delete;
        PhysicSystem (PhysicSystem&& other)			    = delete;
        virtual ~PhysicSystem ()				        = delete;
        PhysicSystem& operator=(PhysicSystem const&)	= delete;
        PhysicSystem& operator=(PhysicSystem &&)		= delete;

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

        static
        void updatePhysicalObjectPointor (PhysicalObject* newPointorPhysicalObject, PhysicalObject* exPointorPhysicalObject) 
        {
            for (std::vector<PhysicalObject*>::iterator it = pPhysicalObjects.begin(); it != pPhysicalObjects.end(); it++)
            {
                if ((*it) == exPointorPhysicalObject)
                {
                    *it = newPointorPhysicalObject;
                    return;
                }
            }
        }

        static
        void removePhysicalObject (PhysicalObject* pPhysicalObject) 
        {
            for (std::vector<PhysicalObject*>::iterator it = pPhysicalObjects.begin(); it != pPhysicalObjects.end(); it++)
            {
                if ((*it) == pPhysicalObject)
                {
                    pPhysicalObjects.erase(it);
                    return;
                }
            }
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