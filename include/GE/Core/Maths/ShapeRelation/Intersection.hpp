//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 15 h 57

#ifndef _INTERSECTION_3D_H
#define _INTERSECTION_3D_H

#include "GE/Core/Maths/vec.hpp"

#include <limits>

namespace Engine::Core::Maths::ShapeRelation
{
    enum class EIntersectionType
    {
        NoIntersection,
        OneIntersectiont,
        TwoIntersectiont,
        InfinyIntersection,
        UnknowIntersection
    };

    struct Intersection
    {
        EIntersectionType intersectionType = EIntersectionType::NoIntersection;
        Vec3 intersection1;
        Vec3 intersection2;
        Vec3 normalI1;
        Vec3 normalI2;

        void SetNotIntersection ()
        {
            intersectionType = EIntersectionType::NoIntersection;
        }

        void SetOneIntersection(const Vec3& intersectionPoint)
        {
            intersectionType = EIntersectionType::OneIntersectiont;
            intersection1 = intersectionPoint;
        }

        void SetTwoIntersection(const Vec3& intersectionPoint1, const Vec3 intersectionPoint2)
        {
            intersectionType = EIntersectionType::TwoIntersectiont;
            intersection1 = intersectionPoint1;
            intersection2 = intersectionPoint2;
        }

        void SetSecondIntersection(const Vec3& intersectionPoint2)
        {
            intersectionType = EIntersectionType::TwoIntersectiont;
            intersection2 = intersectionPoint2;
        }

        void SetInifitIntersection()
        {
            intersectionType = EIntersectionType::InfinyIntersection;
        }

        void SetUnKnowIntersection()
        {
            intersectionType = EIntersectionType::UnknowIntersection;
        }

        void RemoveFirstIntersection()
        {
            if (intersectionType == EIntersectionType::TwoIntersectiont)
            {
                intersectionType = EIntersectionType::OneIntersectiont;
                intersection1 = intersection2;
                normalI1 = normalI2;
            }
            else if (intersectionType == EIntersectionType::OneIntersectiont)
            {
                intersectionType = EIntersectionType::NoIntersection;
            }
        }

        void RemoveSecondIntersection()
        {
            if (intersectionType == EIntersectionType::TwoIntersectiont)
            {
                intersectionType = EIntersectionType::OneIntersectiont;

            }
        }

        /*return true if intersection contenor is full (if there are 2 intersection)*/
        bool AddIntersectionAndCheckIfSecond(const Vec3& intersection)
        {
            if (intersectionType != EIntersectionType::OneIntersectiont)
            {
                SetOneIntersection(intersection);
                return false;
            }
            else
            {
                SetSecondIntersection(intersection);
                return true;
            }
        }

        void SwapIntersection()
        {
            /*swap intersection*/
            Vec3 temp = intersection1;
            intersection1 = intersection2;
            intersection2 = temp;

            /*swap normal*/
            temp = normalI1;
            normalI1 = normalI2;
            normalI2 = temp;
        }

        void SortIntersection(const Vec3& pt1Seg)
        {
            if (intersectionType != EIntersectionType::TwoIntersectiont)
            {
                return;
            }

            if ((intersection1 - pt1Seg).length() > (intersection2 - pt1Seg).length())
            {
                SwapIntersection();
            }
        }

        static bool ComputeDiscriminentAndSolveEquation(float a, float b, float c, const Vec3& segPt1, const Vec3& segPt2, Intersection& intersection)
        {
            float discriminent = b * b - 4.f * a * c;

            if(discriminent < -std::numeric_limits<float>::epsilon()) /*no solution*/
            {
                intersection.SetNotIntersection();
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
                            intersection.SetOneIntersection(t1 * (segPt2 - segPt1) + segPt1);
                        }
                        else
                        {
                            intersection.SetNotIntersection();
                        }

                        if(t2 >= std::numeric_limits<float>::epsilon() && t2 <= 1.f)
                        {
                            if (intersection.intersectionType == EIntersectionType::OneIntersectiont)
                            {
                                intersection.SetSecondIntersection(t2 * (segPt2 - segPt1) + segPt1);
                                return true;
                            }
                            else //second intersection
                            {
                                intersection.SetOneIntersection(t2 * (segPt2 - segPt1) + segPt1);
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
                            intersection.SetOneIntersection(t * (segPt2 - segPt1) + segPt1);
                            return true;
                        }
                        else
                        {
                            intersection.SetNotIntersection();
                            return false;
                        }
                                
                    } 
                }
                else /*inside or outsid the shape*/
                {
                    /*if is insid, infinit solution*/
                    if(c > std::numeric_limits<float>::epsilon())
                    {
                        intersection.SetNotIntersection();
                        return false;
                    }
                }

                intersection.SetInifitIntersection();
                return true;
            }		
        }	
    };
} /*namespace Engine::Core::Maths::ShapeRelation*/

#endif //_INTERSECTION_3D_H