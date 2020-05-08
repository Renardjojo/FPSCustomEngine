//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 13

#ifndef _SEGMENT_SPHERE_H
#define _SEGMENT_SPHERE_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Sphere.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentSphere
    {
        public:

        #pragma region constructor/destructor

        SegmentSphere ()					                    = delete;
        SegmentSphere (const SegmentSphere& other)			    = delete;
        SegmentSphere (SegmentSphere&& other)				    = delete;
        virtual ~SegmentSphere ()				                = delete;
        SegmentSphere& operator=(SegmentSphere const& other)    = delete;
        SegmentSphere& operator=(SegmentSphere && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentSphereCollided(const Shape3D::Segment& seg, const Shape3D::Sphere& sphere, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_SPHERE_H