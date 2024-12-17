using UnityEngine;
using System.Collections.Generic;

/*�K�v�ȃ��C�u����*/
using UnityEditor;          //���j���[�Ȃǂ��o�����߂ɕK�v�Ȃ���
using System.IO;            //�o�͂��邽�߂ɕK�v

//Unity�̌��݂̃V�[�������擾����������
using UnityEngine.SceneManagement; // �R���d�v

public class TrapPlacementDetector : MonoBehaviour
{
    public float gridSize = 1.0f; // �T���v�����O�Ԋu
    public float raycastLength = 10.0f; // Raycast�̒���
    public LayerMask terrainMask; // �n�`���C���[�}�X�N

    [SerializeField] public GameObject minPos;
    [SerializeField] public GameObject maxPos;

    // �ݒu�\���W�̃��X�g
    private List<Vector3> trapPositions = new List<Vector3>();

    void Start()
    {
        DetectTrapPlacementAreas();
        Debug.Log($"���o�����ݒu�\���W: {trapPositions.Count} �ӏ�");

        OutPutTrapPositionData();
    }

    void DetectTrapPlacementAreas()
    {
        // ���A�ǁA�V��̕���
        Vector3[] directions = { Vector3.down, Vector3.forward, Vector3.back, Vector3.left, Vector3.right, Vector3.up };

        // �T���v�����O�͈͂����[�v
        for (float x = minPos.transform.position.x - gridSize / 2; x <= maxPos.transform.position.x; x += gridSize) // X�͈́i�K�X�ύX�j
        {
            for (float y = minPos.transform.position.y - gridSize / 2; y <= maxPos.transform.position.y; y += gridSize) // Y�͈́i�����j
            {
                for (float z = minPos.transform.position.z - gridSize / 2; z <= maxPos.transform.position.z; z += gridSize) // Z�͈́i�K�X�ύX�j
                {
                    Vector3 origin = new Vector3(x, y, z); // �T���v�����O�J�n�_

                    // �e������Raycast�𔭎�
                    foreach (Vector3 dir in directions)
                    {
                        Vector3 retHitPos = Vector3.zero;
                        if (CheckSurface(origin, dir, ref retHitPos))
                        {
                            trapPositions.Add(retHitPos); // �ݒu�\���W��ǉ�
                        }
                    }
                }
            }
        }
    }

    bool CheckSurface(Vector3 origin, Vector3 direction, ref Vector3 ret)
    {
        RaycastHit hit;
        if (Physics.Raycast(origin, direction, out hit, raycastLength, terrainMask))
        {
            // �@���Ŗʂ̌����𔻒�i��: ���A�ǁA�V��j
            Vector3 normal = hit.normal;

            // ����: ���i�������@���j�A�ǁi�������@���j�A�V��i������@���j
            if (IsFloor(normal) || IsWall(normal) || IsCeiling(normal))
            {
                // �I�u�W�F�N�g�ɓ���̃^�O������ꍇ�̂ݗL��
                if (hit.collider.CompareTag("TrapPlaceable"))
                {
                    ret = hit.point;
                    return true;
                }
            }
        }
        return false;
    }

    bool IsFloor(Vector3 normal) => Mathf.Abs(normal.y) > 0.9f; // ��: �@�����قڏ����
    bool IsWall(Vector3 normal) => Mathf.Abs(normal.y) < 0.1f; // ��: �@��������
    bool IsCeiling(Vector3 normal) => normal.y < -0.9f;        // �V��: �@����������

    // Gizmos�Őݒu�\���W��\��
    void OnDrawGizmos()
    {
        Gizmos.color = Color.red;
        foreach (var pos in trapPositions)
        {
            Gizmos.DrawCube(pos, Vector3.one * 0.5f);
        }
    }

    void OutPutTrapPositionData()
    {
        //�o�͐���w�肷��
        string fileName = EditorUtility.SaveFilePanel("�o�̓t�@�C��", "", SceneManager.GetActiveScene().name, "tLoc");

        //�o�͐悪�Ȃɂ��w�肳��Ȃ������Ƃ�(�L�����Z�����ꂽ�Ƃ�)
        if (fileName == "")
        {
            //���������ɏI����
            Debug.LogWarning("�o�͂��܂���ł���");
            return;
        }

        //�t�@�C�����J��
        FileStream fs = File.Create(fileName);

        //�o�C�i���Ƃ��ď�������
        BinaryWriter binaryWriter = new BinaryWriter(fs);

        binaryWriter.Write(trapPositions.Count);

        foreach (var pos in trapPositions)
        {
            WriteVector(binaryWriter, pos);
            Debug.Log(pos);
        }
        //�������ݏI������Close����
        fs.Close();

        return;
    }

    void WriteVector(BinaryWriter bw, Vector3 vec)
    {
        bw.Write(vec.x);
        bw.Write(vec.y);
        bw.Write(vec.z);
    }
}