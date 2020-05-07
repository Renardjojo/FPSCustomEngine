using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class OrientedBoxGO : MonoBehaviour
{
    public Referential referential_;
    public float iI_, iJ_, iK_;

    public OrientedBox OrientedBoxData { get { return new OrientedBox(referential_, iI_, iJ_, iK_); } }

    public Mesh boxMesh;
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
                referential_.unitI = transform.right;
                referential_.unitJ = transform.up;
                referential_.unitK = transform.forward;
                iI_ = transform.localScale.x / 2f;
                iJ_ = transform.localScale.y / 2f;
                iK_ = transform.localScale.z / 2f;

                break;
            case EWitchValueToUse.InpsectorValue:
                transform.position = referential_.origin;
                transform.right = referential_.unitI;
                transform.up = referential_.unitJ;
                transform.forward = referential_.unitK;
                transform.localScale = new Vector3(iI_ * 2f, iJ_ * 2f, iK_ * 2f);
                break;
            default:
                break;
        }
    }


    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawSphere(referential_.origin, gizmosRadius);

        //Quaternion cylinderRot = transform.rotation;
        //cylinderRot.SetFromToRotation(seg_.pt2_, seg_.pt1_);

        Gizmos.DrawMesh(boxMesh, transform.position, transform.rotation, transform.localScale);

        Gizmos.color = Color.red;
        Gizmos.DrawRay(transform.position, referential_.unitI * transform.localScale.magnitude);

        Gizmos.color = Color.green;
        Gizmos.DrawRay(transform.position, referential_.unitJ * transform.localScale.magnitude);

        Gizmos.color = Color.blue;
        Gizmos.DrawRay(transform.position, referential_.unitK * transform.localScale.magnitude);
    }
}

public class OrientedBox
{
    /**
     * @brief Construct a new OrientedBox object
     * 
     * @param referential   :   A central point and three unit vectors, two by two 
     *                          orthogonal. Two of the vectors are collinear alongside the quad and the third
     *                          vector represents the vector normal to the plane of the rectangle.
     * @param iI            :   lengths in i vector
     * @param iJ            :   lengths in j vector
     * @param iK            :   lengths in k vector
     */
    public OrientedBox(Referential referential, float iI, float iJ, float iK)
    {
        referential_ = referential;
        iI_ = iI;
        iJ_ = iJ;
        iK_ = iK;
    }

    public Referential referential_;
    public float iI_, iJ_, iK_;
}
