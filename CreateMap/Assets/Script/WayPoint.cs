using UnityEngine;
using System.Collections.Generic;

public class Waypoint : MonoBehaviour
{
    public List<GameObject> connectedWaypoints; // �ڑ����鎟�̃E�F�C�|�C���g

    void OnDrawGizmos()
    {
        // �G�f�B�^�[�Ŏ��o�I�Ɋm�F���邽�߂̐���`��
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