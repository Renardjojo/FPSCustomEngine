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
        Vec3 	origin;
        Vec3	unitI; //x
        Vec3	unitJ; //y
        Vec3	unitK; //z

        static Vec3 GlobalToLocalPosition(Referential refLocal, Vec3 point)
        {
            //O'Mw 	= OMw  - OO'w
            Vec3 OlocalM = point - refLocal.origin;

            //x = O'Mw * i'w 
            //y = O'Mw * j'w
            //z = O'Mw * k'w
            return Vec3({OlocalM.dotProduct(refLocal.unitI), OlocalM.dotProduct(refLocal.unitJ), OlocalM.dotProduct(refLocal.unitK)});
        }

        static Vec3 LocalToGlobalPosition(Referential refLocal, Vec3 point)
        {
            //O'Mw  = x' * i' + y' * j' + z' * k'
            Vec3 vectorOlocalM = refLocal.unitI * point.x + refLocal.unitJ * point.y + refLocal.unitK * point.z;

            //OM = OO' + O'M
            return refLocal.origin  + vectorOlocalM;
        }
    };

} /*namespace Engine::Core::Maths */


#endif //_REFERENTIAL_H