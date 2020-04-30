//Project : Math
//Editing by Six Jonathan
//Date : 31/01/2020 - 17 h 19

#ifndef _PRIMITIVE3D_H
#define _PRIMITIVE3D_H

#include <memory>
#include <math.h>
#include <limits>
#include <iomanip>
#include <vector>

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/mat.hpp"
#include "GE/Core/Maths/plane.hpp"
#include "GE/Core/Maths/Referential.hpp"

namespace Engine::Core::Maths
{
    class Segment
    {
        public :

        Segment ()                             = default;
        Segment(const Segment& other)          = default;
        Segment(Segment&& other)               = default;
        ~Segment()                             = default;
        Segment& operator=(Segment const&)     = default;
        Segment& operator=(Segment &&)         = default; 

        explicit Segment (const Vec3& pt1, const Vec3& pt2)
            :   pt1_    {pt1},
                pt2_    {pt2}
        {}

        protected :

        Vec3      pt1_, 
                        pt2_;
    };

    class Line
    {
        public :

        Line ()                             = default;
        Line(const Line& other)             = default;
        Line(Line&& other)            = default;
        ~Line()                             = default;
        Line& operator=(Line const&)  = default;
        Line& operator=(Line &&)      = default; 

        explicit Line (const Vec3& pt, const Vec3& normal)
            :   pt_     {pt},
                normal_ {normal}
        {}

        public :

        Vec3    pt_, 
                normal_;
    };

    class Quad
    {
        public :

        Quad ()                                 = default;
        Quad(const Quad& other)                 = default;
        Quad(Quad&& other)                      = default;
        ~Quad()                                 = default;
        Quad& operator=(Quad const&)      = default;
        Quad& operator=(Quad &&)          = default;

        /**
         * @brief Construct a new Quad object
         * 
         * @param referential   :   A central point and three unit vectors, two by two 
         *                          orthogonal. Two of the vectors are collinear alongside the quad and the third
         *                          vector represents the vector normal to the plane of the rectangle.
         * @param iI            :   lengths in i vector
         * @param iJ            :   lengths in j vector
         */
         Quad (const Referential& referential, float iI, float iJ)
            :   referential_    {referential},
                iI_             {iI},
                iJ_             {iJ}
        {}

        /**
         * @brief Get the Plan object on the quad
         * 
         * @return Plan 
         */
        
        Plane getPlane()
        {
            return Plane(referential_.origin, referential_.unitK);
        }

        protected :

        Referential     referential_;
        float           iI_, iJ_;
    };

    class InfiniteCylindre
    {
        public :

        InfiniteCylindre ()                                                     = default;
        InfiniteCylindre(const InfiniteCylindre& other)                         = default;
        InfiniteCylindre( InfiniteCylindre&& other)                             = default;
        ~InfiniteCylindre()                                                     = default;
        InfiniteCylindre& operator=(InfiniteCylindre const&)  = default;
        InfiniteCylindre& operator=(InfiniteCylindre &&)      = default; 

        explicit InfiniteCylindre (const Line& line, float radius)
            :   line_    {line},
                radius_  {radius}
        {}

        public :

        Line    line_;
        float   radius_;
    };

    class Cylindre
    {
        public :

        Cylindre ()                           = default;
        Cylindre(const Cylindre& other)       = default;
        Cylindre( Cylindre&& other)           = default;
        ~Cylindre()                           = default;
        Cylindre& operator=(Cylindre const&)  = default;
        Cylindre& operator=(Cylindre &&)      = default; 

        explicit Cylindre (const Segment& seg, float radius)
            :   segment_ {seg},
                radius_  {radius}
        {}

        public :

        Segment segment_;
        float   radius_;
    };

    class Sphere
    {
        public :

        Sphere ()                                   = default;
        Sphere(const Sphere& other)                 = default;
        Sphere(Sphere&& other)                      = default;
        ~Sphere()                                   = default;
        Sphere& operator=(Sphere const&)    = default;
        Sphere& operator=(Sphere &&)        = default; 

        explicit Sphere (const Vec3& center, float radius)
            :   center_    {center},
                radius_    {radius}
        {}

        const Vec3&     getCenter() { return center_;}
        float           getRadius() { return radius_;}

        void    setCenter(const Vec3& center)   { center_ = center;}
        void    setRadius(float radius)         { radius_ = radius;}

        protected :

        Vec3    center_;
        float   radius_;
    };

    class Capsule
    {
        public :

        Capsule ()                                   = default;
        Capsule(const Capsule& other)                 = default;
        Capsule(Capsule&& other)                      = default;
        ~Capsule()                                   = default;
        Capsule& operator=(Capsule const&)    = default;
        Capsule& operator=(Capsule &&)        = default; 

        explicit Capsule (const Vec3& center, const Vec3& normal, float height, float radius)
            :   seg_        {Vec3(center - normal * (height / 2.f)), Vec3(center + normal * (height / 2.f))},
                radius_     {radius}
        {}

        explicit Capsule (const Segment& seg, float radius)
            :   seg_        {seg},
                radius_     {radius}
        {}

        protected :

        Segment     seg_;
        float       radius_;
    };

    class AABB
    {
        public :

        AABB ()                                 = default;
        AABB(const AABB& other)                 = default;
        AABB(AABB&& other)                      = default;
        ~AABB()                                 = default;
        AABB& operator=(AABB const&)            = default;
        AABB& operator=(AABB &&)                = default; 

        explicit AABB (const Vec3& center, float upLenght, float rightLenght, float forwardLenght)
            :   ref_    {center, Vec3::up, Vec3::right, Vec3::forward},
                iI_     (upLenght), 
                iJ_     (rightLenght), 
                iK_     (forwardLenght)
        {}

        std::vector<Quad> getFaces ()
        {
             return std::vector<Quad>{  Quad(Referential{ref_.origin + ref_.unitI,  ref_.unitI, ref_.unitJ, ref_.unitK}, iJ_, iK_), 
                                        Quad(Referential{ref_.origin - ref_.unitI, -ref_.unitI, ref_.unitJ,-ref_.unitK}, iJ_, iK_), 
                                        Quad(Referential{ref_.origin + ref_.unitJ,  ref_.unitJ,-ref_.unitI, ref_.unitK}, iK_, iI_),
                                        Quad(Referential{ref_.origin - ref_.unitJ, -ref_.unitJ, ref_.unitI, ref_.unitK}, iK_, iI_),
                                        Quad(Referential{ref_.origin + ref_.unitK,  ref_.unitK, ref_.unitJ,-ref_.unitI}, iI_, iJ_),
                                        Quad(Referential{ref_.origin - ref_.unitK, -ref_.unitK, ref_.unitJ, ref_.unitI}, iI_, iJ_)};
        }

        protected :

        Referential ref_;
        float       iI_, iJ_, iK_; 
    };

    class OrientedBox
    {
        public :

        OrientedBox ()                              = default;
        OrientedBox(const OrientedBox& other)       = default;
        OrientedBox(OrientedBox&& other)            = default;
        ~OrientedBox()                              = default;
        OrientedBox& operator=(OrientedBox const&)  = default;
        OrientedBox& operator=(OrientedBox &&)      = default;

        explicit OrientedBox (const Vec3& center, const Vec3& rotation, float upLenght, float rightLenght, float forwardLenght)
            :   ref_            {center},
                iI_     (upLenght), 
                iJ_  (rightLenght), 
                iK_(forwardLenght)
        {
            Mat3 rotationMatrix = Mat3::createFixedAngleEulerRotationMatrix(rotation);
            ref_.unitI = rotationMatrix.getVectorUp();
            ref_.unitJ = rotationMatrix.getVectorRight();
            ref_.unitK = rotationMatrix.getVectorForward();
        }

        std::vector<Quad> getFaces ()
        {
             return std::vector<Quad>{  Quad(Referential{ref_.origin + ref_.unitI,  ref_.unitI, ref_.unitJ, ref_.unitK}, iJ_, iK_), 
                                        Quad(Referential{ref_.origin - ref_.unitI, -ref_.unitI, ref_.unitJ,-ref_.unitK}, iJ_, iK_), 
                                        Quad(Referential{ref_.origin + ref_.unitJ,  ref_.unitJ,-ref_.unitI, ref_.unitK}, iK_, iI_),
                                        Quad(Referential{ref_.origin - ref_.unitJ, -ref_.unitJ, ref_.unitI, ref_.unitK}, iK_, iI_),
                                        Quad(Referential{ref_.origin + ref_.unitK,  ref_.unitK, ref_.unitJ,-ref_.unitI}, iI_, iJ_),
                                        Quad(Referential{ref_.origin - ref_.unitK, -ref_.unitK, ref_.unitJ, ref_.unitI}, iI_, iJ_)};
        }


        const Referential&      getReferential()                { return ref_;}
        float                   getExtensionValueI()             { return iI_;}
        float                   getExtensionValueJ()             { return iJ_;}
        float                   getExtensionValueK()             { return iK_;}

        void                    setReferential(const Referential& newRef)   { ref_  = newRef;}
        void                    setExtensionValueI(float iI)                { iI_   = iI;}
        void                    setExtensionValueJ(float iJ)                { iJ_   = iJ;}
        void                    etExtensionValueK(float iK)                 { iK_   = iK;}

        protected :

        Referential ref_;
        float       iI_, iJ_, iK_; 
    };
}

#endif // _PRIMITIVE3D_H
