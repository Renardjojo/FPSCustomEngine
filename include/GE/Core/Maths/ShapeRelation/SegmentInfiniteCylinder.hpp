//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 17 h 20

#ifndef _SEGMENT_INFINITE_CYLINDER_H
#define _SEGMENT_INFINITE_CYLINDER_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/InfiniteCylinder.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentInfiniteCylinder
    {
        public:

        #pragma region constructor/destructor

        SegmentInfiniteCylinder ()					                                = delete;
        SegmentInfiniteCylinder (const SegmentInfiniteCylinder& other)			    = delete;
        SegmentInfiniteCylinder (SegmentInfiniteCylinder&& other)				    = delete;
        virtual ~SegmentInfiniteCylinder ()				                            = delete;
        SegmentInfiniteCylinder& operator=(SegmentInfiniteCylinder const& other)    = delete;
        SegmentInfiniteCylinder& operator=(SegmentInfiniteCylinder && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentInfiniteCylinderCollided(const Shape3D::Segment& seg, const Shape3D::InfiniteCylinder& infCylinder, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_INFINITE_CYLINDER_H