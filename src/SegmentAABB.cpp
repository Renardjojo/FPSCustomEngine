#include "GE/Core/Maths/ShapeRelation/SegmentAABB.hpp"
#include "GE/Core/Maths/MathTools.hpp"
#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

bool SegmentAABB::isSegmentAABBCollided(Segment seg, AABB AABB, Intersection& intersection)
{
    Vec3 AB = seg.getPt2() - seg.getPt1();
    float tx0, tx1, ty0, ty1, tz0, tz1, tempT;
    tempT = 1.f; // memorise with temporal float the value of T. Compare this value to the new T and compute the nearest point af seg.pt1

    intersection.intersectionType = EIntersectionType::NoIntersection;

    if (!isBetween(AB.x, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
    {
        float invABX = 1.f / AB.x;
        tx0 = (AABB.getExtI() - seg.getPt1().x + AABB.getCenter().x) * invABX;
        tx1 = (-AABB.getExtI() - seg.getPt1().x + AABB.getCenter().x) * invABX;

        addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tx0, tempT, Vec3::right);

        if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tx1, tempT, Vec3::left))
        {
            return true;
        }
    }

    if (!isBetween(AB.y, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
    {
        float invABY = 1.f / AB.y;
        ty0 = (AABB.getExtJ() - seg.getPt1().y + AABB.getCenter().y) * invABY;
        ty1 = (-AABB.getExtJ() - seg.getPt1().y + AABB.getCenter().y) * invABY;

        if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, ty0, tempT, Vec3::up))
        {
            return true;
        }

        if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, ty1, tempT, Vec3::down))
        {
            return true;
        }
    }

    if (!isBetween(AB.z, -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon()))
    {
        float invABZ = 1.f / AB.z;
        tz0 = (AABB.getExtK() - seg.getPt1().z + AABB.getCenter().z) * invABZ;
        tz1 = (-AABB.getExtK() - seg.getPt1().z + AABB.getCenter().z) * invABZ;

        if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tz0, tempT, Vec3::forward))
        {
            return true;
        }

        if (addIntersectionWithScalerIfOnAABBAndReturnIfFull(seg, AABB, intersection, tz1, tempT, Vec3::backward))
        {
            return true;
        }
    }

    return (intersection.intersectionType == EIntersectionType::OneIntersectiont);
}

bool SegmentAABB::addIntersectionWithScalerIfOnAABBAndReturnIfFull(const Segment& seg, const AABB& AABB, Intersection& intersection, float scaler, float& tempT, Vec3 faceNormal)
{
    if (isBetween(scaler, 0.f, 1.f))
    {
        Vec3 pt = seg.getPt1() + (scaler * (seg.getPt2() - seg.getPt1()));

        if (AABB.isInside(pt))
        {
            if (intersection.intersectionType != EIntersectionType::OneIntersectiont)
            {
                intersection.setOneIntersection(pt);
                intersection.normalI1 = faceNormal;

                tempT = scaler;
                return false;
            }
            else
            {
                intersection.setSecondIntersection(pt);
                intersection.normalI2 = faceNormal;

                if (tempT > scaler)
                {
                    intersection.swapIntersection();
                }
                return true;
            }
        }
    }
    
    return false;
}