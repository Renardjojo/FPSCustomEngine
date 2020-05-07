using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class AABBGO : MonoBehaviour
{
    public Vector3 center_;
    public float iI_, iJ_, iK_;

    public AABB AABBData { get { return new AABB(center_, iI_, iJ_, iK_); } }

    public Mesh boxMesh;
    public Mesh sphereMesh;
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
                center_ = transform.position;
                iI_ = transform.localScale.x / 2f;
                iJ_ = transform.localScale.y / 2f;
                iK_ = transform.localScale.z / 2f;

                break;
            case EWitchValueToUse.InpsectorValue:
                transform.position = center_;

                transform.localScale = new Vector3(iI_ * 2f, iJ_ * 2f, iK_ * 2f);
                break;
            default:
                break;
        }
    }


    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawSphere(center_, gizmosRadius);

        //Quaternion cylinderRot = transform.rotation;
        //cylinderRot.SetFromToRotation(seg_.pt2_, seg_.pt1_);

        Gizmos.DrawMesh(boxMesh, transform.position, Quaternion.Euler(0f, 0f, 0f) , transform.localScale);

        Gizmos.color = Color.red;
        Gizmos.DrawRay(transform.position, Vector3.right * transform.localScale.magnitude);

        Gizmos.color = Color.green;
        Gizmos.DrawRay(transform.position, Vector3.up * transform.localScale.magnitude);

        Gizmos.color = Color.blue;
        Gizmos.DrawRay(transform.position, Vector3.forward * transform.localScale.magnitude);
    }
}

public class AABB
{
    public AABB(Vector3 center, float iI, float iJ, float iK)
    {
        center_ = center;
        iI_ = iI;
        iJ_ = iJ;
        iK_ = iK;
    }

    public Vector3 center_;
    public float iI_, iJ_, iK_;
}
