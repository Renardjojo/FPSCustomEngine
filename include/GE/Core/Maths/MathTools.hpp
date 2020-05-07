//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 10 h 35

#ifndef _MATHS_TOOLS_H
#define _MATHS_TOOLS_H

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths
{
    #define isBetween(number, low, up) (number >= low && number <= up)

    // static public bool ComputeDiscriminentAndSolveEquation(float a, float b, float c, Segment seg, Intersection intersection)
	// {
    //     float discriminent = b * b - 4f * a * c;

    //     if(discriminent < -Mathf.Epsilon) /*no solution*/
    //     {
    //         intersection.SetNotIntersection();
    //         return false;
    //     }
    //     else  /*1, 2 or infinite intersections*/
    //     {
    //         if (a > Mathf.Epsilon)
    //         {
    //             if (discriminent > Mathf.Epsilon) /*2 intersections with the line*/
    //             {
                    
    //                 float rootDiscriminent = Mathf.Sqrt(discriminent);
    //                 float t1 = (-b - rootDiscriminent) / (2f * a);
    //                 float t2 = (-b + rootDiscriminent) / (2f * a);

    //                 /*Check if t1 and t2 is between [0, 1]. So if the intersections belongs to the segment**/
    //                 if (t1 >= Mathf.Epsilon && t1 <= 1f)
    //                 {
    //                     intersection.SetOneIntersection(t1 * (seg.pt2_ - seg.pt1_) + seg.pt1_);
    //                 }
    //                 else
    //                 {
    //                     intersection.SetNotIntersection();
    //                 }

    //                 if(t2 >= Mathf.Epsilon && t2 <= 1f)
    //                 {
    //                     if (intersection.intersectionType == EIntersectionType.OneIntersectiont)
    //                     {
    //                         intersection.SetSecondIntersection(t2 * (seg.pt2_ - seg.pt1_) + seg.pt1_);
    //                         return true;
    //                     }
    //                     else //second intersection
    //                     {
    //                         intersection.SetOneIntersection(t2 * (seg.pt2_ - seg.pt1_) + seg.pt1_);
    //                         return true;
    //                     }
    //                 }

    //                 if (intersection.intersectionType == EIntersectionType.NoIntersection)
    //                 {
    //                     /*Check if segment is insid or outside the shape*/
    //                     if ((t1 > 1f && t2 > 1f) || (t1 < 0f && t2 < 0f))
    //                     {
    //                         return false;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     return true;
    //                 }
    //             }
    //             else /*1 intersection with the line*/
    //             {
    //                 float t = -b / (2f * a);

    //                 /*Check if t is between [0, 1]. So if the intersection belongs to the segment*/
    //                 if(t >= Mathf.Epsilon && t <= 1f)
    //                 {
    //                     intersection.SetOneIntersection(t * (seg.pt2_ - seg.pt1_) + seg.pt1_);
    //                     return true;
    //                 }
    //                 else
    //                 {
    //                     intersection.SetNotIntersection();
    //                     return false;
    //                 }
                            
    //             } 
    //         }
    //         else /*inside or outsid the shape*/
    //         {
    //             /*if is insid, infinit solution*/
    //             if(c > Mathf.Epsilon)
    //             {
    //                 intersection.SetNotIntersection();
    //                 return false;
    //             }
    //         }

    //         intersection.SetInifitIntersection();
    //         return true;
    //     }		
	// }	
}

#endif //!_MATHS_TOOLS_H
