#include "GE/Core/Maths/ShapeRelation/SegmentQuad.hpp"

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentPlane.hpp"
#include "GE/Core/Maths/ShapeRelation/SegmentSegment.hpp"

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentQuad::isSegmentQuadCollided(const Segment& seg, const Quad& quad, Intersection& intersection)
{
    /*We need to know the intersection*/
    Plane planOfQuad = static_cast<Plane>(quad);

    if (!SegmentPlane::isSegmentPlaneCollided(seg, planOfQuad, intersection))
    {
        intersection.setNotIntersection();
        return false;
    }

    /*Check if the segment is on the plan*/
    if (intersection.intersectionType == EIntersectionType::InfinyIntersection)
    {
        /*This is a plan problem that must be solve with SAT algorythme in 2 dimension*/
        /*This case is imposible with 3d trajectory*/
        intersection.setUnKnowIntersection();
        return detectCollisionOn2DQuad(quad, seg, intersection);
    }

    /*Check if intersection is insid the quad*/
    if (std::abs(Vec3::dot(quad.getReferential().unitI, intersection.intersection1 - quad.getReferential().origin)) <= quad.getExtI() &&
        std::abs(Vec3::dot(quad.getReferential().unitJ, intersection.intersection1 - quad.getReferential().origin)) <= quad.getExtJ())
    {
        return true;
    }

    intersection.setNotIntersection();
    return false;

}

int SegmentQuad::detectPointEmplacement (const Vec2& point, const Vec2& min, const Vec2& max)
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

bool SegmentQuad::detectCollisionOn2DQuad(const Quad& quad, const Segment& seg, Intersection& intersection)
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
            intersection.setInifitIntersection();
            return true;
        }
        else
        {
            intersection.setNotIntersection();
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
        intersection.setNotIntersection();
        return false;
    }

    bool rst;

    //TODO : Add each case to optimise thiw code and juste check the good segment

    intersection.setNotIntersection();
    rst =  SegmentSegment::isSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtTopLeft(),      quad.PtTopRight(),    intersection);
    rst |= SegmentSegment::isSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtTopRight(),     quad.PtBottomRight(), intersection);
    rst |= SegmentSegment::isSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtBottomRight(),  quad.PtBottomLeft(),  intersection);
    rst |= SegmentSegment::isSegmentSegmentCollided(seg.getPt1(), seg.getPt2(), quad.PtBottomLeft(),   quad.PtTopLeft(),     intersection);

    return rst;
}