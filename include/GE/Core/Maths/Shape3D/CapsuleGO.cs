using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class CapsuleGO : MonoBehaviour
{ 
    public SegmentGO  seg_;
    public float    radius_;

    public Mesh cylinderMesh;
    public Mesh sphereMesh;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;

    public Capsule CapsuleData { get { return new Capsule(seg_.pt1_, seg_.pt2_, radius_); } }

    private void Update()
    {}

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Quaternion capsuleRot = seg_.transform.rotation;
        capsuleRot.SetLookRotation(seg_.transform.up, seg_.transform.right);

        Gizmos.DrawMesh(sphereMesh, seg_.pt1_, transform.rotation, new Vector3(radius_ * 2f, radius_ * 2f, radius_ * 2f));
        Gizmos.DrawMesh(sphereMesh, seg_.pt2_, transform.rotation, new Vector3(radius_ * 2f, radius_ * 2f, radius_ * 2f));
        Gizmos.DrawMesh(cylinderMesh, seg_.pt1_ + (seg_.pt2_ - seg_.pt1_) / 2f, capsuleRot, new Vector3(radius_ * 2f, (seg_.pt2_ - seg_.pt1_).magnitude / 2f, radius_ * 2f));
     }
}

public class Capsule
{
    public Capsule(Vector3 pt1, Vector3 pt2, float radius)
    {
        seg_ = new Segment(pt1, pt2);
        radius_ = radius;
    }

    public Capsule(Vector3 center, Vector3 normal, float height, float radius)
    {
        seg_ = new Segment(center - normal * (height / 2f), (center + normal * (height / 2f)));
        radius_    = radius;
    }

    public Segment seg_;
    public float radius_;

    public Mesh capsuleMesh;
    public Color color = Color.white;

    public Color hitColor = Color.red;
    internal bool isHit = false;

    private void Update()
    { }

    /**
     * @brief Get the InfiniteCapsule object on the capsule
     * 
     * @return InfiniteCapsule 
     */
    public InfiniteCylinder getInfiniteCylinder()
    {
        return new InfiniteCylinder(seg_.getLine(), radius_);
    }

    public Cylinder BodyCylinder
    {
        get
        {
            return new Cylinder(seg_.pt1_, seg_.pt2_, radius_);
        }
    }

    public Sphere LeftSphere
    {
        get
        {
            return new Sphere(radius_, seg_.pt1_);
        }
    }

    public Sphere RightSphere
    {
        get
        {
            return new Sphere(radius_, seg_.pt2_);
        }
    }
}