using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class LineGO : MonoBehaviour
{
    public Vector3  pt_;
    public Vector3  normal_;

    public Line LineData { get { return new Line(pt_, normal_); } }

    public float gizmosRadius = 1f;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;
    public EWitchValueToUse useNormalValueOrRotationValue;

    private void Update()
    {
        switch (useNormalValueOrRotationValue)
        {
            case EWitchValueToUse.InpsectorValue:
                transform.position = pt_;
                transform.forward = normal_;
                break;
            case EWitchValueToUse.TransformeValue:
                pt_ = transform.localPosition;
                normal_ = transform.forward;
                break;
            default:
                break;
        }
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawSphere(pt_, gizmosRadius);
        Gizmos.DrawRay(pt_ - (normal_ * 500f), normal_ * 1000f);
        DrawArrow.ForGizmo(pt_, normal_ * 5f);
    }
}

public class Line
{
    public Line(Vector3 pt, Vector3 normal)
    {
        pt_ = pt;
        normal_ = normal.normalized;
    }

    public Vector3 pt_;
    public Vector3 normal_;
}