using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

[ExecuteInEditMode]
public class QuadGO : MonoBehaviour
{
    public Mesh quadMesh;

    public Referential referential_;
    public float iI_, iJ_;

    public Quad QuadData { get { return new Quad(referential_, iI_,  iJ_); } }

    public float gizmosRadius = 1f;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;
    public EWitchValueToUse valueToUse;

    private void Update()
    {
        switch (valueToUse)
        {
            case EWitchValueToUse.TransformeValue:
                referential_.origin = transform.position;
                referential_.unitI  = transform.right;
                referential_.unitJ  = transform.up;
                referential_.unitK  = transform.forward;
                iI_                 = transform.localScale.x / 2f;
                iJ_                 = transform.localScale.z / 2f;

                break;
            case EWitchValueToUse.InpsectorValue:
                transform.position  = referential_.origin;
                transform.right     = referential_.unitI;
                transform.up        = referential_.unitJ;
                transform.forward   = referential_.unitK;
                transform.localScale = new Vector3(iI_ * 2f, transform.localScale.y, iJ_ * 2f);
                break;
            default:
                break;
        }
    }


    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawSphere(referential_.origin, gizmosRadius);
        Quaternion quadRot = Quaternion.Euler(0f, 0f, 0f);
        quadRot.SetLookRotation(referential_.unitJ, referential_.unitK);
        Gizmos.DrawMesh(quadMesh, referential_.origin, quadRot, new Vector3(iI_ *2f, 0.01f, iJ_ * 2f));

        Quaternion quadRot2 = Quaternion.Euler(0f, 0f, 0f);
        quadRot2.SetLookRotation(referential_.unitJ, -referential_.unitK);
        Gizmos.DrawMesh(quadMesh, referential_.origin, quadRot2, new Vector3(iI_ *2f, 0.01f, iJ_ * 2f));


        Gizmos.color = Color.red;
        Gizmos.DrawRay(referential_.origin, referential_.unitI * new Vector3(iI_ *2f, 1f, iJ_ * 2f).magnitude);

        Gizmos.color = Color.green;
        Gizmos.DrawRay(referential_.origin, referential_.unitJ * new Vector3(iI_ *2f, 1f, iJ_ * 2f).magnitude);

        Gizmos.color = Color.blue;
        Gizmos.DrawRay(referential_.origin, referential_.unitK * new Vector3(iI_ *2f, 1f, iJ_ * 2f).magnitude);
    }
}

public class Quad
{
    public static int QUAD_OUTCODE_INSIDE = 0;   // 0000
    public static int QUAD_OUTCODE_LEFT = 1;     // 0001
    public static int QUAD_OUTCODE_RIGHT = 2;    // 0010
    public static int QUAD_OUTCODE_BOTTOM = 4;   // 0100
    public static int QUAD_OUTCODE_TOP = 8;      // 1000

    /**
     * @brief Construct a new Quad object
     * 
     * @param referential   :   A central point and three unit vectors, two by two 
     *                          orthogonal. Two of the vectors are collinear alongside the quad and the third
     *                          vector represents the vector normal to the plane of the rectangle.
     * @param iI            :   lengths in i vector
     * @param iJ            :   lengths in j vector
     */
    public Quad(Referential referential, float iI, float iJ)
    {
        referential_ = referential;
        iI_ = iI;
        iJ_ = iJ;
    }

    public bool isPointInside(Vector3 pt)
    {
        return  MathTools.IsBetween(Vector3.Dot(referential_.unitI, pt - referential_.origin), -iI_, iI_) &&
                MathTools.IsBetween(Vector3.Dot(referential_.unitJ, pt - referential_.origin), -iJ_, iJ_) &&
                MathTools.IsBetween(Vector3.Dot(referential_.unitK, pt - referential_.origin), -Mathf.Epsilon, Mathf.Epsilon);
    }

    public bool isPointInsideQuadZone(Vector3 pt)
    {
        return MathTools.IsBetween(Vector3.Dot(referential_.unitI, pt - referential_.origin), -iI_, iI_) &&
                MathTools.IsBetween(Vector3.Dot(referential_.unitJ, pt - referential_.origin), -iJ_, iJ_);
    }

    public int isPointInsideQuadZoneOutCode(Vector3 pt)
    {
        int outCode = 0;
        float projectionPtOnI = Vector3.Dot(referential_.unitI, pt - referential_.origin);
        float projectionPtOnJ = Vector3.Dot(referential_.unitJ, pt - referential_.origin);
        outCode += projectionPtOnI <  iI_ ? 0 : QUAD_OUTCODE_RIGHT;
        outCode += projectionPtOnI > -iI_ ? 0 : QUAD_OUTCODE_LEFT;
        outCode += projectionPtOnJ <  iJ_ ? 0 : QUAD_OUTCODE_TOP;
        outCode += projectionPtOnJ > -iJ_ ? 0 : QUAD_OUTCODE_BOTTOM;
        return outCode;
    }

    public Referential referential_;
    public float iI_, iJ_;
    public Vector3 PtTopLeft { get { return referential_.origin - (referential_.unitI * iI_) + (referential_.unitJ * iJ_); } }
    public Vector3 PtTopRight { get { return referential_.origin + (referential_.unitI * iI_) + (referential_.unitJ * iJ_); } }
    public Vector3 PtBottomLeft { get { return referential_.origin - (referential_.unitI * iI_) - (referential_.unitJ * iJ_); } }
    public Vector3 PtBottomRight { get { return referential_.origin + (referential_.unitI * iI_) - (referential_.unitJ * iJ_); } }

    /**
     * @brief Get the Plan object on the quad
     * 
     * @return Plan 
     */
    public Plan getPlane()
    {
        return new Plan(referential_.origin, referential_.unitK);
    }
}