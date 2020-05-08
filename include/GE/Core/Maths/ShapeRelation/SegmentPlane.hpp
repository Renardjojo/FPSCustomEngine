//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_PLANE_H
#define _SEGMENT_PLANE_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentPlane
    {
        public:

        #pragma region constructor/destructor

        SegmentPlane ()					                    = delete;
        SegmentPlane (const SegmentPlane& other)			= delete;
        SegmentPlane (SegmentPlane&& other)				    = delete;
        virtual ~SegmentPlane ()				            = delete;
        SegmentPlane& operator=(SegmentPlane const& other)  = delete;
        SegmentPlane& operator=(SegmentPlane && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentPlaneCollided(const Shape3D::Segment& seg, const Shape3D::Plane& plane, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_PLANE_H