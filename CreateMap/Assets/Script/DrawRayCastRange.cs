using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DrawRayCastRange : MonoBehaviour
{
    public GameObject start;
    public GameObject end;

    void OnDrawGizmos()
    {
        // エディターで視覚的に確認するための線を描画
        Gizmos.color = Color.green;
        if (start != null && end != null)
        {
            Vector3 startPos = start.transform.position;
            Vector3 endPos = end.transform.position;
            Vector3 center = (startPos + endPos) / 2;
            Vector3 size = new Vector3(Mathf.Abs(startPos.x - endPos.x), Mathf.Abs(startPos.y - endPos.y), Mathf.Abs(startPos.z - endPos.z));
            Gizmos.DrawWireCube(center, size);
        }
    }
}
