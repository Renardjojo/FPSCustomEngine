//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-12 - 15 h 13

#ifndef _AABB_AABB_H
#define _AABB_AABB_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class AabbAabb
    {
        public:

        #pragma region constructor/destructor

        AabbAabb ()					                = delete;
        AabbAabb (const AabbAabb& other)			= delete;
        AabbAabb (AabbAabb&& other)				    = delete;
        virtual ~AabbAabb ()				        = delete;
        AabbAabb& operator=(AabbAabb const& other)  = delete;
        AabbAabb& operator=(AabbAabb && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isBothAABBCollided(const Shape3D::AABB& aabb1, const Shape3D::AABB& aabb2, Intersection& intersection);
        static bool isBothAABBCollided(const Shape3D::AABB& aabb1, const Shape3D::AABB& aabb2);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_AABB_AABB_H