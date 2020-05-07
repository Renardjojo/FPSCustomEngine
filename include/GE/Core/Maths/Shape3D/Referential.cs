using System.Collections;
using System.Collections.Generic;
using UnityEngine;


[System.Serializable]
public struct Referential
{
    public Vector3 origin;
    public Vector3 unitI; //x
    public Vector3 unitJ; //y
    public Vector3 unitK; //z

    static public Vector3 GlobalToLocalPosition(Referential refLocal, Vector3 point)
    {
        //O'Mw 	= OMw  - OO'w
        Vector3 OlocalM = point - refLocal.origin;

        //x = O'Mw * i'w 
        //y = O'Mw * j'w
        //z = O'Mw * k'w
        return new Vector3(Vector3.Dot(OlocalM, refLocal.unitI), Vector3.Dot(OlocalM, refLocal.unitJ), Vector3.Dot(OlocalM, refLocal.unitK));
    }

    static public Vector3 LocalToGlobalPosition(Referential refLocal, Vector3 point)
    {
        //O'Mw  = x' * i' + y' * j' + z' * k'
        Vector3 vectorOlocalM = refLocal.unitI * point.x + refLocal.unitJ * point.y + refLocal.unitK * point.z;

        //OM = OO' + O'M
        return refLocal.origin  + vectorOlocalM;
    }

    static public Vector3 GlobalToLocalVector(Referential refLocal, Vector3 vector)
    {
        //U'x = U * i
        //U'y = U * j
        //U'z = U * k
        return new Vector3( Vector3.Dot(vector, refLocal.unitI),
                            Vector3.Dot(vector, refLocal.unitJ),
                            Vector3.Dot(vector, refLocal.unitK));
    }

    static public Vector3 LocalToGlobalVector(Referential refLocal, Vector3 vector)
    {
        //vect(U) = u'x * i'(world) + u'y * j'(world) + u'z * k'(world)
        return (refLocal.unitI * vector.x) + (refLocal.unitJ * vector.y) + (refLocal.unitK * vector.z);
    }
};
