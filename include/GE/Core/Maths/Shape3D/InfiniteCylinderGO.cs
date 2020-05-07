using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class InfiniteCylinderGO : MonoBehaviour
{ 
    public LineGO line_;
    public float radius_;

    public InfiniteCylinder CylinderData { get { return new InfiniteCylinder(line_.LineData, radius_); } }

    public Mesh cylinderMesh;
    public Color color = Color.white;
    public float cylinderScale = 10f;

    public Color hitColor = Color.red;
    internal bool isHit = false;
    public EWitchValueToUse useNormalValueOrRotationValue;

    private void Update()
    {
        switch (useNormalValueOrRotationValue)
        {
            case EWitchValueToUse.InpsectorValue:
                transform.localScale = new Vector3(radius_ * 2f, cylinderScale, radius_ * 2f);
                break;
            case EWitchValueToUse.TransformeValue:
                radius_ = transform.localScale.x * 2f;
                break;
            default:
                break;
        }
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Quaternion cylinderRot = line_.transform.rotation;
        cylinderRot.SetLookRotation(line_.transform.up, line_.transform.forward);

        Gizmos.DrawMesh(cylinderMesh, line_.pt_, cylinderRot, new Vector3(radius_ * 2f, cylinderScale, radius_ * 2f));
    }
}

public class InfiniteCylinder
{
    public InfiniteCylinder(Vector3 pt, Vector3 normal, float radius)
    {
        line_ = new Line(pt, normal.normalized);
        radius_ = radius;
    }

    public InfiniteCylinder(Line line, float radius)
    {
        line_ = line;
        radius_ = radius;
    }

    public bool IsPointInside(Vector3 pt)
    {
        return Vector3.Cross(line_.normal_, pt - line_.pt_).magnitude <= radius_;
    }

    public Line line_;
    public float radius_;
}