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
    class Line
    {
        public : 

        Line ()                             = default;
        Line(const Line& other)             = default;
        Line(Line&& other)                  = default;
        ~Line()                             = default;
        Line& operator=(Line const&)        = default;
        Line& operator=(Line &&)            = default; 

        explicit Line (const Vec3& origin, const Vec3& normal)
            :   origin_     {origin},
                normal_ {normal}
        {}

        const Vec3& getOrigin() noexcept    { return origin_; }
        const Vec3& getNormal() noexcept    { return normal_; }

        void setOrigin(const Vec3& newOrigin) noexcept { origin_ = newOrigin; }
        void setNormal(const Vec3& newNormal) noexcept { normal_ = newNormal; }

        protected :

        Vec3    origin_, 
                normal_;
    };

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

        const Vec3& getPt1() noexcept { return pt1_; }
        const Vec3& getPt2() noexcept { return pt2_; }

        void setPt1(const Vec3& newPt) noexcept { pt1_ = newPt; }
        void setPt2(const Vec3& newPt) noexcept { pt2_ = newPt; }

        explicit operator Line () //use static_cast<Line>(seg) to convert seg to Line
        {
            return Line(pt1_, (pt2_ - pt1_).normalize()); 
        }

        protected :

        Vec3      pt1_, pt2_;
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
        explicit operator Plane() //use static_cast<Plane>(quad) to convert quad to plane
        {
            return Plane(referential_.origin, referential_.unitK);
        }

        protected :

        Referential     referential_;
        float           iI_, iJ_;
    };

    class InfiniteCylinder
    {
        public :

        InfiniteCylinder ()                                                     = default;
        InfiniteCylinder(const InfiniteCylinder& other)                         = default;
        InfiniteCylinder( InfiniteCylinder&& other)                             = default;
        ~InfiniteCylinder()                                                     = default;
        InfiniteCylinder& operator=(InfiniteCylinder const&)  = default;
        InfiniteCylinder& operator=(InfiniteCylinder &&)      = default; 

        explicit InfiniteCylinder (const Line& line, float radius)
            :   line_    {line},
                radius_  {radius}
        {}

        public :

        Line    line_;
        float   radius_;
    };

    class Cylinder
    {
        public :

        Cylinder ()                           = default;
        Cylinder(const Cylinder& other)       = default;
        Cylinder( Cylinder&& other)           = default;
        ~Cylinder()                           = default;
        Cylinder& operator=(Cylinder const&)  = default;
        Cylinder& operator=(Cylinder &&)      = default; 

        explicit Cylinder (const Segment& seg, float radius)
            :   segment_ {seg},
                radius_  {radius}
        {}

        explicit operator InfiniteCylinder() //use static_cast<InfiniteCylinder>(seg) to convert Cylinder to InfiniteCylinder
        {
            return InfiniteCylinder(static_cast<Line>(segment_), radius_);
        }

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
        virtual ~Sphere()                           = default;
        Sphere& operator=(Sphere const&)    = default;
        Sphere& operator=(Sphere &&)        = default; 

        explicit Sphere (float radius, const Vec3& localCenter = Vec3::zero)
            :   center_        {localCenter},
                radius_             {radius}
        {}

        virtual Vec3      getCenter()         { return center_;}
        virtual float     getRadius()         { return radius_;}

        void    setCenter(const Vec3& center)   { center_ = center;}
        void    setRadius(float radius)         { radius_ = radius;}

        protected :

        Vec3            center_;
        float           radius_;
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

        explicit AABB (float upLenght, float rightLenght, float forwardLenght, const Vec3& center = Vec3::zero)
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
        virtual ~OrientedBox()                      = default;
        OrientedBox& operator=(OrientedBox const&)  = default;
        OrientedBox& operator=(OrientedBox &&)      = default;

        explicit OrientedBox (float rightLenght, float upLenght, float forwardLenght, const Vec3& center = Vec3::zero, const Vec3& rotation = Vec3::zero)
            :   ref_    {center},
                iI_     (rightLenght), 
                iJ_     (upLenght), 
                iK_     (forwardLenght)
        {
            Mat3 rotationMatrix = Mat3::createFixedAngleEulerRotationMatrix(rotation);
            ref_.unitI = rotationMatrix.getVectorRight();
            ref_.unitJ = rotationMatrix.getVectorUp();
            ref_.unitK = rotationMatrix.getVectorForward();
        }

        virtual std::vector<Quad> getFaces ()
        {
            const Referential& ref = getReferential();  //include virtual modification
            float iI = getExtensionValueI();            //include virtual modification
            float iJ = getExtensionValueJ();            //include virtual modification
            float iK = getExtensionValueK();            //include virtual modification

            return std::vector<Quad>{  Quad(Referential{ref.origin + ref.unitI,  ref.unitI, ref.unitJ, ref.unitK}, iJ, iK), 
                                        Quad(Referential{ref.origin - ref.unitI, -ref.unitI, ref.unitJ,-ref.unitK}, iJ, iK), 
                                        Quad(Referential{ref.origin + ref.unitJ,  ref.unitJ,-ref.unitI, ref.unitK}, iK, iI),
                                        Quad(Referential{ref.origin - ref.unitJ, -ref.unitJ, ref.unitI, ref.unitK}, iK, iI),
                                        Quad(Referential{ref.origin + ref.unitK,  ref.unitK, ref.unitJ,-ref.unitI}, iI, iJ),
                                        Quad(Referential{ref.origin - ref.unitK, -ref.unitK, ref.unitJ, ref.unitI}, iI, iJ)};
        }
        
        virtual Referential             getReferential()                { return ref_;}
        virtual float                   getExtensionValueI()            { return iI_;}
        virtual float                   getExtensionValueJ()            { return iJ_;}
        virtual float                   getExtensionValueK()            { return iK_;}
 
        void                    setReferential(const Referential& newRef)   { ref_  = newRef;}
        void                    setExtensionValueI(float iI)                { iI_   = iI;}
        void                    setExtensionValueJ(float iJ)                { iJ_   = iJ;}
        void                    setExtensionValueK(float iK)                 { iK_   = iK;}

        protected :

        Referential ref_;
        float       iI_, iJ_, iK_; 
    };
}

#endif // _PRIMITIVE3D_H
