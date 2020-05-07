using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public enum EWitchValueToUse
{
    InpsectorValue,
    TransformeValue
}

[ExecuteInEditMode]
public class PlanGO : MonoBehaviour
{
    public Mesh planMesh;

    public Vector3  normal_;                    //unit vector
    public float    distance_;                  //Distance with origin

    public Plan PlanData { get { return new Plan(distance_, normal_.normalized); } }

    public float gizmosRadius = 1f;
    public Color color = Color.white;
    public float planScale = 10f;

    public Color hitColor = Color.red;
    internal bool isHit = false;
    public EWitchValueToUse useNormalValueOrRotationValue;

    private void Update()
    {
        switch (useNormalValueOrRotationValue)
        {
            case EWitchValueToUse.InpsectorValue:
                transform.forward = normal_.normalized;
                break;
            case EWitchValueToUse.TransformeValue:
                normal_ = transform.forward;
                break;
            default:
                break;
        }

        distance_ = Vector3.Dot(transform.position, normal_.normalized);
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawSphere(normal_.normalized * distance_, gizmosRadius);
        Quaternion planRot = transform.rotation;
        planRot.SetLookRotation(transform.up, transform.forward);
        Gizmos.DrawMesh(planMesh, normal_.normalized * distance_, planRot, new Vector3(planScale, 0.01f, planScale));

        Quaternion planRot2 = transform.rotation;
        planRot2.SetLookRotation(transform.up, -transform.forward);
        Gizmos.DrawMesh(planMesh, normal_.normalized * distance_, planRot2, new Vector3(planScale, 0.01f, planScale));


        DrawArrow.ForGizmo(normal_.normalized * distance_, normal_ * 5f);
    }
}

public class Plan
{
    public Plan(float distance, Vector3 normal)
    {
        normal_ = normal.normalized;
        distance_ = distance;
    }

    public Plan(Vector3 p1, Vector3 p2, Vector3 p3)
    {
        normal_ = Vector3.Cross((p2 - p1), p3 - p1).normalized;
        distance_ = Vector3.Dot(p1, normal_);
    }

    public Plan(Vector3 pt, Vector3 normal)
    {
        normal_ = normal.normalized;
        distance_ = Vector3.Dot(pt, normal_);
    }

    public Vector3 normal_;                    //unit vector
    public float distance_;                  //Distance with origin


    public float getSignedDistanceToPlane(Vector3 point)
    {
        return Vector3.Dot(point, normal_) - distance_;
    }

    static public bool vecIsBetweenPlanes(Plan plane1, Plan plane2, Vector3 pt)
    {
        return (plane1.getSignedDistanceToPlane(pt) <= -Mathf.Epsilon) && (plane2.getSignedDistanceToPlane(pt) <= -Mathf.Epsilon);
    }

}