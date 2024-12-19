using UnityEngine;
using System.Collections.Generic;

public class Waypoint : MonoBehaviour
{
    public List<GameObject> connectedWaypoints; // 接続する次のウェイポイント

    void OnDrawGizmos()
    {
        // エディターで視覚的に確認するための線を描画
        Gizmos.color = Color.blue;
        if (connectedWaypoints != null)
        {
            foreach (var wp in connectedWaypoints)
            {
                if (wp != null)
                {
                    Gizmos.DrawLine(transform.position, wp.transform.position);
                }
            }
        }
    }
}