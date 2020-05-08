#include "GE/Core/Maths/ShapeRelation/Intersection.hpp"
#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::ShapeRelation;

bool Intersection::computeDiscriminentAndSolveEquation(float a, float b, float c, const Vec3& segPt1, const Vec3& segPt2, Intersection& intersection)
{
    float discriminent = b * b - 4.f * a * c;

    if(discriminent < -std::numeric_limits<float>::epsilon()) /*no solution*/
    {
        intersection.setNotIntersection();
        return false;
    }
    else  /*1, 2 or infinite intersections*/
    {
        if (a > std::numeric_limits<float>::epsilon())
        {
            if (discriminent > std::numeric_limits<float>::epsilon()) /*2 intersections with the line*/
            {
                
                float rootDiscriminent = std::sqrt(discriminent);
                float t1 = (-b - rootDiscriminent) / (2.f * a);
                float t2 = (-b + rootDiscriminent) / (2.f * a);

                /*Check if t1 and t2 is between [0, 1]. So if the intersections belongs to the segment**/
                if (t1 >= std::numeric_limits<float>::epsilon() && t1 <= 1.f)
                {
                    intersection.setOneIntersection(t1 * (segPt2 - segPt1) + segPt1);
                }
                else
                {
                    intersection.setNotIntersection();
                }

                if(t2 >= std::numeric_limits<float>::epsilon() && t2 <= 1.f)
                {
                    if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                    {
                        intersection.setSecondIntersection(t2 * (segPt2 - segPt1) + segPt1);
                        return true;
                    }
                    else //second intersection
                    {
                        intersection.setOneIntersection(t2 * (segPt2 - segPt1) + segPt1);
                        return true;
                    }
                }

                if (intersection.intersectionType == EIntersectionType::NoIntersection)
                {
                    /*Check if segment is insid or outside the shape*/
                    if ((t1 > 1.f && t2 > 1.f) || (t1 < 0.f && t2 < 0.f))
                    {
                        return false;
                    }
                }
                else
                {
                    return true;
                }
            }
            else /*1 intersection with the line*/
            {
                float t = -b / (2.f * a);

                /*Check if t is between [0, 1]. So if the intersection belongs to the segment*/
                if(t >= std::numeric_limits<float>::epsilon() && t <= 1.f)
                {
                    intersection.setOneIntersection(t * (segPt2 - segPt1) + segPt1);
                    return true;
                }
                else
                {
                    intersection.setNotIntersection();
                    return false;
                }
                        
            } 
        }
        else /*inside or outsid the shape*/
        {
            /*if is insid, infinit solution*/
            if(c > std::numeric_limits<float>::epsilon())
            {
                intersection.setNotIntersection();
                return false;
            }
        }

        intersection.setInifitIntersection();
        return true;
    }		
}	