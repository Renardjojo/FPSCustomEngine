﻿//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SPHERE_ORIENTED_BOX_H
#define _SPHERE_ORIENTED_BOX_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SphereOrientedBox
    {
        public:

        #pragma region constructor/destructor

        SphereOrientedBox ()					                        = delete;
        SphereOrientedBox (const SphereOrientedBox& other)			    = delete;
        SphereOrientedBox (SphereOrientedBox&& other)				    = delete;
        virtual ~SphereOrientedBox ()				                    = delete;
        SphereOrientedBox& operator=(SphereOrientedBox const& other)    = delete;
        SphereOrientedBox& operator=(SphereOrientedBox && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSphereOrientedBoxCollided(const Shape3D::Sphere& sphere, const Shape3D::OrientedBox& box, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SPHERE_ORIENTED_BOX_H