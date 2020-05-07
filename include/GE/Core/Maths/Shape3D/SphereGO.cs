using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class SphereGO : MonoBehaviour
{
    public Mesh sphereMesh;

    public Vector3  center_;                    //unit vector
    public float    radius_;                  //Distance with origin

    public Sphere SphereData { get { return new Sphere(radius_, center_); } }

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
                transform.position = center_;
                transform.localScale = new Vector3(radius_ * 2f, radius_ * 2f, radius_ * 2f);
                break;
            case EWitchValueToUse.TransformeValue:
                center_ = transform.position;
                radius_ = transform.localScale.x / 2f;
                break;
            default:
                break;
        }
    }


    private void OnDrawGizmos()
    {
        Gizmos.color = isHit ? hitColor : color;

        Gizmos.DrawMesh(sphereMesh, transform.position, transform.rotation, new Vector3(radius_ * 2f, radius_ * 2f, radius_ * 2f));
    }
}

public class Sphere
{
    public Sphere(float radius, Vector3 center)
    {
        radius_ = radius;
        center_ = center;
    }

    public Vector3 center_;                    //unit vector
    public float radius_;                  //Distance with origin
}