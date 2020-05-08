//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_ORIENTED_BOX_H
#define _SEGMENT_ORIENTED_BOX_H

#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    class SegmentOrientedBox
    {
        public:

        #pragma region constructor/destructor

        SegmentOrientedBox ()					                        = delete;
        SegmentOrientedBox (const SegmentOrientedBox& other)			= delete;
        SegmentOrientedBox (SegmentOrientedBox&& other)				    = delete;
        virtual ~SegmentOrientedBox ()				                    = delete;
        SegmentOrientedBox& operator=(SegmentOrientedBox const& other)  = delete;
        SegmentOrientedBox& operator=(SegmentOrientedBox && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool isSegmentOrientedBoxCollided(const Shape3D::Segment& seg, const Shape3D::OrientedBox& orientedBox, Intersection& intersection);

        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_ORIENTED_BOX_H