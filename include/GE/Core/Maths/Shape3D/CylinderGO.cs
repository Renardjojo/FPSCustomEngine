using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class CylinderGO : MonoBehaviour
{ 
    public SegmentGO  seg_;
    public float    radius_;

    public Mesh cylinderMesh;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;

    public Cylinder CylinderData { get { return new Cylinder(seg_.pt1_, seg_.pt2_, radius_); } }

    private void Update()
    {}

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Quaternion cylinderRot = seg_.transform.rotation;
        cylinderRot.SetLookRotation(seg_.transform.up, seg_.transform.right);

        Gizmos.DrawMesh(cylinderMesh, seg_.pt1_ + (seg_.pt2_ - seg_.pt1_) / 2f, cylinderRot, new Vector3(radius_ * 2f, (seg_.pt2_ - seg_.pt1_).magnitude / 2f, radius_ * 2f));      
    }
}

public class Cylinder
{
    public Cylinder(Vector3 pt1, Vector3 pt2, float radius)
    {
        seg_ = new Segment(pt1, pt2);
        radius_ = radius;
    }

    public Segment seg_;
    public float radius_;

    public Mesh cylinderMesh;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;

    private void Update()
    { }

    /**
     * @brief Get the InfiniteCylinder object on the cylinder
     * 
     * @return InfiniteCylinder 
     */
    public InfiniteCylinder getInfiniteCylinder()
    {
        return new InfiniteCylinder(seg_.getLine(), radius_);
    }

    public Plan LeftPlane
    {
        get
        {
            return new Plan(seg_.pt1_, (seg_.pt1_ - seg_.pt2_).normalized);
        }
    }

    public Plan RightPlane
    {
        get
        {
            return new Plan(seg_.pt2_, (seg_.pt2_ - seg_.pt1_).normalized);
        }
    }
}