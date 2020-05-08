//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_SEGMENT_H
#define _SEGMENT_SEGMENT_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentSegment
    {
        public:

        #pragma region constructor/destructor

        SegmentSegment ()					                    = delete;
        SegmentSegment (const SegmentSegment& other)			= delete;
        SegmentSegment (SegmentSegment&& other)				    = delete;
        virtual ~SegmentSegment ()				                = delete;
        SegmentSegment& operator=(SegmentSegment const& other)  = delete;
        SegmentSegment& operator=(SegmentSegment && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentSegmentCollided(const Vec3& a1, const Vec3& a2, const Vec3& b1, const Vec3& b2, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_SEGMENT_H