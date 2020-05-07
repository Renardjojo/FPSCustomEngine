//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 16 h 05

#ifndef _SEGMENT_QUAD_H
#define _SEGMENT_QUAD_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include "GE/Core/Maths/Shape3D/Segment.hpp"
#include "GE/Core/Maths/Shape3D/Quad.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentSegment.hpp"

namespace Engine::Core::Maths::ShapeRelation
{
    #define INSIDE  0   // 0000
    #define LEFT    1   // 0001
    #define RIGHT   2   // 0010
    #define BOTTOM  4   // 0100
    #define TOP     8   // 1000

    #define CENTRAL_COLMN_MASK  2  // 0011
    #define CENTRAL_LINE_MASK   12 // 1100

    class SegmentQuad
    {
        public:

        #pragma region constructor/destructor

        SegmentQuad ()					                    = delete;
        SegmentQuad (const SegmentQuad& other)			    = delete;
        SegmentQuad (SegmentQuad&& other)				    = delete;
        virtual ~SegmentQuad ()				                = delete;
        SegmentQuad& operator=(SegmentQuad const& other)    = delete;
        SegmentQuad& operator=(SegmentQuad && other)		= delete;

        #pragma endregion //!constructor/destructor

        #pragma region static methods

        static bool IsSegmentQuadCollided(const Shape3D::Segment& seg, const Shape3D::Quad& quad, Intersection& intersection)
        {
            /*We need to know the intersection*/
            Shape3D::Plane planOfQuad = static_cast<Shape3D::Plane>(quad);

            if (!SegmentPlane::IsSegmentPlanCollided(seg, planOfQuad, intersection))
            {
                intersection.SetNotIntersection();
                return false;
            }

            /*Check if the segment is on the plan*/
            if (intersection.intersectionType == EIntersectionType::InfinyIntersection)
            {
                /*This is a plan problem that must be solve with SAT algorythme in 2 dimension*/
                /*This case is imposible with 3d trajectory*/
                intersection.SetUnKnowIntersection();
                return DetectCollisionOn2DQuad(quad, seg, intersection);
            }

            /*Check if intersection is insid the quad*/
            if (std::abs(Vec3::dot(quad.getReferential().unitI, intersection.intersection1 - quad.getReferential().origin)) <= quad.getExtI() &&
                std::abs(Vec3::dot(quad.getReferential().unitJ, intersection.intersection1 - quad.getReferential().origin)) <= quad.getExtJ())
            {
                return true;
            }

            intersection.SetNotIntersection();
            return false;

        }

        #pragma endregion //!static methods

        private :

        #pragma region static methods

        /*using the Cohen–Sutherland algorithm to detect the emplacement of the point*/
        static int detectPointEmplacement (const Vec2& point, const Vec2& min, const Vec2& max)
        {
            int code = INSIDE;      // initialised as being inside of [[clip window]]

            if (point.x < min.x)           // to the left of clip window
            {
                code |= LEFT;
            }
            else if (point.x > max.x)      // to the right of clip window
            {
                code |= RIGHT;
            }

            if (point.y < min.y)           // below the clip window
            {
                code |= BOTTOM;
            }
            else if (point.y > max.y)      // above the clip window
            {
                code |= TOP;
            }

            return code;
        }

        static bool DetectCollisionOn2DQuad(const Shape3D::Quad& quad, const Shape3D::Segment& seg, Intersection& intersection)
        {
            //Keep on float for more visibility
            float   rangeMinAxisX   = Vec3::dot(quad.getReferential().origin + (-quad.getReferential().unitI * quad.getExtI()), quad.getReferential().unitI), 
                    rangeMaxAxisX   = rangeMinAxisX + 2.f * quad.getExtI(),
                    rangeMinAxisY   = Vec3::dot(quad.getReferential().origin + (-quad.getReferential().unitJ * quad.getExtJ()), quad.getReferential().unitJ),
                    rangeMaxAxisY   = rangeMinAxisY + 2.f * quad.getExtJ(), 
                    rangePoint1X    = Vec3::dot(seg.getPt1(), quad.getReferential().unitI),
                    rangePoint1Y    = Vec3::dot(seg.getPt1(), quad.getReferential().unitJ),
                    rangePoint2X    = Vec3::dot(seg.getPt2(), quad.getReferential().unitI),
                    rangePoint2Y    = Vec3::dot(seg.getPt2(), quad.getReferential().unitJ);

            Vec2 rangeMinXY {rangeMinAxisX, rangeMinAxisY};
            Vec2 rangeMaxXY {rangeMaxAxisX, rangeMaxAxisY};
            Vec2 rangePt1   {rangePoint1X, rangePoint1Y};
            Vec2 rangePt2   {rangePoint2X, rangePoint2Y};

            int emplacementCode1 = detectPointEmplacement(rangePt1, rangeMinXY, rangeMaxXY);
            int emplacementCode2 = detectPointEmplacement(rangePt2, rangeMinXY, rangeMaxXY);

            //                      OutCode : 
            //          left     |  central  |   right
            //          ---------------------------------
            //  top     |1001 (9)|   1000 (8)|   1010 (10)
            //  central |0001 (1)|   0000 (0)|   0010 (2)
            //  bottom  |0101 (5)|   0100 (4)|   0110 (6)

            //check if two point is inside the same zone
            if (emplacementCode1 == emplacementCode2)
            {
                //If the emplacement first emplacement is Inside (the second is obligatory in same zone) there is infinit intersection. Else no collsion
                if(emplacementCode1 == INSIDE)
                {
                    intersection.SetInifitIntersection();
                    return true;
                }
                else
                {
                    intersection.SetNotIntersection();
                    return false;
                }
            }

            //check if two point are on the same border. Apply the mask of the border and compare it
            //If true, there cannot be a collision
            if (((emplacementCode1 & TOP) == TOP && (emplacementCode2 & TOP) == TOP) ||
                    ((emplacementCode1 & RIGHT) == RIGHT && (emplacementCode2 & RIGHT) == RIGHT) ||
                    ((emplacementCode1 & LEFT) == LEFT && (emplacementCode2 & LEFT) == LEFT) ||
                    ((emplacementCode1 & BOTTOM) == BOTTOM && (emplacementCode2 & BOTTOM) == BOTTOM))
            {
                intersection.SetNotIntersection();
                return false;
            }

            bool rst;

            //TODO : Add each case to optimise thiw code and juste check the good segment

            intersection.SetNotIntersection();
            rst =  SegmentSegment::IsSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtTopLeft(),      quad.PtTopRight(),    intersection);
            rst |= SegmentSegment::IsSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtTopRight(),     quad.PtBottomRight(), intersection);
            rst |= SegmentSegment::IsSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtBottomRight(),  quad.PtBottomLeft(),  intersection);
            rst |= SegmentSegment::IsSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtBottomLeft(),   quad.PtTopLeft(),     intersection);

            return rst;
        }

        #pragma endregion //!static methods
    };

} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_SEGMENT_QUAD_H