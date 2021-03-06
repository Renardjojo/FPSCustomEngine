//Project : Engine
//Editing by Gavelle Anthony, Nisi Guillaume, Six Jonathan
//Date : 2020-04-30 - 10 h 35

#ifndef _REFERENTIAL_H
#define _REFERENTIAL_H

#include "GE/Core/Maths/vec.hpp"

namespace Engine::Core::Maths
{
    struct Referential
    {
		Referential ()								        = default;
		Referential (const Referential& other)			    = default;
		Referential (Referential&& other)					= default;
		~Referential ()							            = default;
		Referential& operator=(Referential const& other)    = default;
		Referential& operator=(Referential && other)		= default;

        Vec3 	origin  {Vec3::zero};
        Vec3	unitI   {Vec3::right};      //x
        Vec3	unitJ   {Vec3::up};         //y
        Vec3	unitK   {Vec3::forward};    //z

        static Vec3 globalToLocalPosition(const Referential& refLocal, const Vec3& point)
        {
            //O'Mw 	= OMw  - OO'w
            Vec3 OlocalM = point - refLocal.origin;

            //x = O'Mw * i'w 
            //y = O'Mw * j'w
            //z = O'Mw * k'w
            return Vec3({OlocalM.dotProduct(refLocal.unitI), OlocalM.dotProduct(refLocal.unitJ), OlocalM.dotProduct(refLocal.unitK)});
        }

        static Vec3 localToGlobalPosition(const Referential&  refLocal, const Vec3& point)
        {
            //O'Mw  = x' * i' + y' * j' + z' * k'
            Vec3 vectorOlocalM = refLocal.unitI * point.x + refLocal.unitJ * point.y + refLocal.unitK * point.z;

            //OM = OO' + O'M
            return refLocal.origin  + vectorOlocalM;
        }

        static Vec3 globalToLocalVector(const Referential&  refLocal, const Vec3& vector)
        {
            //U'x = U * i
            //U'y = U * j
            //U'z = U * k
            return Vec3({Vec3::dot(vector, refLocal.unitI), Vec3::dot(vector, refLocal.unitJ), Vec3::dot(vector, refLocal.unitK)});
        }

        static Vec3 localToGlobalVector(const Referential&  refLocal, const Vec3& vector)
        {
            //vect(U) = u'x * i'(world) + u'y * j'(world) + u'z * k'(world)
            return (refLocal.unitI * vector.x) + (refLocal.unitJ * vector.y) + (refLocal.unitK * vector.z);
        }

    };

} /*namespace Engine::Core::Maths */


#endif //_REFERENTIAL_H