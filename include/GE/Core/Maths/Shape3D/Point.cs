using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class Point : MonoBehaviour
{
    public float gizmosRadius = 0.5f;
    public Color color = Color.white;

    public Point(Vector3 pt)
    {
        transform.position = pt;
    }

    private void OnDrawGizmos()
    {
        Gizmos.color = color;

        Gizmos.DrawSphere(transform.position, gizmosRadius);
    }
}