//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_ORIENTED_BOX_H
#define _SEGMENT_ORIENTED_BOX_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/OrientedBox.hpp"
#include "GE/Core/Maths/Shape3D/AABB.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentAABB.hpp"

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

        static bool IsSegmentOrientedBoxCollided(const Shape3D::Segment& seg, const Shape3D::OrientedBox& orientedBox, Intersection& intersection)
        {
            /*Transform the segment from global referential to the local referential of the oriented box*/
            Shape3D::Segment localSegment { Referential::GlobalToLocalPosition(orientedBox.getReferential(), seg.getPt1()),
                                            Referential::GlobalToLocalPosition(orientedBox.getReferential(), seg.getPt2())};

            Shape3D::AABB orientedBoxAxisAligned {Referential::GlobalToLocalPosition(orientedBox.getReferential(), orientedBox.getReferential().origin), orientedBox.getExtI(), orientedBox.getExtJ(), orientedBox.getExtK()};

            if(SegmentAABB::IsSegmentAABBCollided(localSegment, orientedBoxAxisAligned, intersection))
            {
                intersection.intersection1 = Referential::LocalToGlobalPosition(orientedBox.getReferential(), intersection.intersection1);
                
                if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
                    intersection.intersection2 = Referential::LocalToGlobalPosition(orientedBox.getReferential(), intersection.intersection2);

                intersection.normalI1 = Referential::LocalToGlobalVector(orientedBox.getReferential(), intersection.normalI1);

                if (intersection.intersectionType == EIntersectionType::TwoIntersectiont)
                {
                    intersection.normalI2 = Referential::LocalToGlobalVector(orientedBox.getReferential(), intersection.normalI2);
                }

                return true;
            }

            intersection.intersectionType = EIntersectionType::NoIntersection;
            return false;
        }


        #pragma endregion //!static methods

        private :

        #pragma region static methods
        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_ORIENTED_BOX_H