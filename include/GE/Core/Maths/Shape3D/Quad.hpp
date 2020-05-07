
//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-05-07 - 13 h 12

#ifndef _QUAD_H
#define _QUAD_H

#include "GE/Core/Maths/vec.hpp"
#include "GE/Core/Maths/Shape3D/Plane.hpp"
#include "GE/Core/Maths/Referential.hpp"
#include "GE/Core/Maths/MathTools.hpp"

#include <limits>

namespace Engine::Core::Maths::Shape3D
{
    #define QUAD_OUTCODE_INSIDE   0;  // 0000
    #define QUAD_OUTCODE_LEFT     1;  // 0001
    #define QUAD_OUTCODE_RIGHT    2;  // 0010
    #define QUAD_OUTCODE_BOTTOM   4;  // 0100
    #define QUAD_OUTCODE_TOP      8;  // 1000
    
    class Quad
    {
        public:
    
        #pragma region constructor/destructor
    
        Quad ()                       = default;
        Quad(const Quad& other)       = default;
        Quad( Quad&& other)           = default;
        virtual ~Quad()               = default;
        Quad& operator=(Quad const&)  = default;
        Quad& operator=(Quad &&)      = default;

        /**
         * @brief Construct a new Quad object
         * 
         * @param referential   :   A central point and three unit vectors, two by two 
         *                          orthogonal. Two of the vectors are collinear alongside the quad and the third
         *                          vector represents the vector normal to the plane of the rectangle.
         * @param iI            :   lengths in i vector
         * @param iJ            :   lengths in j vector
         */
         explicit Quad (const Referential& referential, float iI, float iJ)
            :   referential_    {referential},
                iI_             {iI},
                iJ_             {iJ}
        {}

        #pragma endregion //!constructor/destructor
    
        #pragma region methods

        Vec3 PtTopLeft      ()  const noexcept  { return referential_.origin - (referential_.unitI * iI_) + (referential_.unitJ * iJ_); }
        Vec3 PtTopRight     ()  const noexcept  { return referential_.origin + (referential_.unitI * iI_) + (referential_.unitJ * iJ_); }
        Vec3 PtBottomLeft   ()  const noexcept  { return referential_.origin - (referential_.unitI * iI_) - (referential_.unitJ * iJ_); }
        Vec3 PtBottomRight  ()  const noexcept  { return referential_.origin + (referential_.unitI * iI_) - (referential_.unitJ * iJ_); }

        bool isPointInside(const Vec3& pt) const noexcept
        {
            return  isBetween(Vec3::dot(referential_.unitI, pt - referential_.origin), -iI_, iI_) &&
                    isBetween(Vec3::dot(referential_.unitJ, pt - referential_.origin), -iJ_, iJ_) &&
                    isBetween(Vec3::dot(referential_.unitK, pt - referential_.origin), -std::numeric_limits<float>::epsilon(), std::numeric_limits<float>::epsilon());
        }

        bool isPointInsideQuadZone(const Vec3& pt) const noexcept 
        {
            return  isBetween(Vec3::dot(referential_.unitI, pt - referential_.origin), -iI_, iI_) &&
                    isBetween(Vec3::dot(referential_.unitJ, pt - referential_.origin), -iJ_, iJ_);
        }

        int isPointInsideQuadZoneOutCode(const Vec3& pt) const noexcept
        {
            int outCode = 0;
            float projectionPtOnI = Vec3::dot(referential_.unitI, pt - referential_.origin);
            float projectionPtOnJ = Vec3::dot(referential_.unitJ, pt - referential_.origin);
            outCode += projectionPtOnI <  iI_ ? 0 : QUAD_OUTCODE_RIGHT;
            outCode += projectionPtOnI > -iI_ ? 0 : QUAD_OUTCODE_LEFT;
            outCode += projectionPtOnJ <  iJ_ ? 0 : QUAD_OUTCODE_TOP;
            outCode += projectionPtOnJ > -iJ_ ? 0 : QUAD_OUTCODE_BOTTOM;
            return outCode;
        }

        #pragma endregion //!methods
    
        #pragma region accessor

        const Referential&  getReferential()    const noexcept  { return referential_; }
        Referential&        getReferential()          noexcept  { return referential_; }
        const float&        getExtI()           const noexcept  { return iI_; }
        const float&        getExtJ()           const noexcept  { return iJ_; }

        #pragma endregion //!accessor

        #pragma region mutator

        void setReferential (const Referential& newReferential)    noexcept   {  referential_ = newReferential; }
        void setExtI        (const float& newExtI)                 noexcept   {  iI_ = newExtI; }
        void setExtJ        (const float& newExtJ)                 noexcept   {  iJ_ = newExtJ; }

        #pragma endregion //!mutator

        #pragma region convertor

        explicit operator Plane() //use static_cast<Plane>(quad) to convert quad to plane
        {
            return Plane(referential_.origin, referential_.unitK);
        }

        #pragma endregion //!convertor
    
        protected:
    
        #pragma region attribut

        Referential referential_;
        float iI_, iJ_;

        #pragma endregion //!attribut

        #pragma region static attribut

        #pragma endregion //! static attribut

        private:
    
    };

} /*namespace Engine::Core::Maths::Shape3D*/

#endif //_QUAD_H