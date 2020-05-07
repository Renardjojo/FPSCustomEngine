using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class SegmentGO : MonoBehaviour
{
    public Point pt1;
    public Point pt2;

    internal Vector3 pt1_;
    internal Vector3 pt2_;

    public Segment SegmentData { get { return new Segment(pt1_, pt2_); } }

    public float gizmosRadius = 1f;
    public Color color = Color.white;
    public Color hitColor = Color.red;
    public bool centerOnVector = true;

    internal bool isHit = false;

    private void Update()
    {
        pt1.color = color;
        pt2.color = color;

        pt1.gizmosRadius = gizmosRadius;
        pt2.gizmosRadius = gizmosRadius;

        pt1_ = pt1.transform.position;
        pt2_ = pt2.transform.position;

        if (centerOnVector)
        {
            transform.position = pt1_ + ((pt2_ - pt1_) / 2f);
        }
    }

    /**
     * @brief Get the InfiniteCylinder object on the cylinder
     * 
     * @return InfiniteCylinder 
     */
    public Line getLine()
    {
        return new Line(pt1_, (pt2_ - pt1_).normalized);
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawLine(pt1_, pt2_);
    }
}

public class Segment
{
    public Segment(Vector3 _pt1, Vector3 _pt2)
    {
        pt1_ = _pt1;
        pt2_ = _pt2;
    }

    internal Vector3 pt1_;
    internal Vector3 pt2_;

    public float getDistanceWithPoint(Vector3 pt)
    {
        Vector3 AB = pt2_ - pt1_;

        if (LeftPlane.getSignedDistanceToPlane(pt) > Mathf.Epsilon)
        {
            if(RightPlane.getSignedDistanceToPlane(pt) > Mathf.Epsilon)
            {
                Vector3 AC = pt - pt1_;
                //The point in between the 2 plan. So The result is the rejection of the vectoo AC on AB
                return (AC - ((Vector3.Dot(AC, AB) / Vector3.Dot(AB, AB)) * AB)).magnitude;
}
            else
            {
                //The point is behind the plane of point B. The result is the length of the vector BC
                return (pt - pt2_).magnitude;
            }
        }
        else
        {
            //The point is behind the plane of point A. The result is the length of the vector AC
            return (pt - pt1_).magnitude;
        }
    }

    /**
     * @brief Get the InfiniteCylinder object on the cylinder
     * 
     * @return InfiniteCylinder 
     */
    public Line getLine()
    {
        return new Line(pt1_, (pt2_ - pt1_).normalized);
    }

    public Plan LeftPlane
    {
        get
        {
            return new Plan(pt1_, (pt1_ - pt2_).normalized);
        }
    }

    public Plan RightPlane
    {
        get
        {
            return new Plan(pt2_, (pt2_ - pt1_).normalized);
        }
    }
}