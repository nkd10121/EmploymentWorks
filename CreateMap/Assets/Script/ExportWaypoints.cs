using UnityEngine;
using System.Collections.Generic;

/*�K�v�ȃ��C�u����*/
using UnityEditor;          //���j���[�Ȃǂ��o�����߂ɕK�v�Ȃ���
using System.IO;            //�o�͂��邽�߂ɕK�v

//Unity�̌��݂̃V�[�������擾����������
using UnityEngine.SceneManagement; // �R���d�v

public class ExportWaypoints : MonoBehaviour
{
    private List<GameObject> waypoints = new List<GameObject>();

    public void Start()
    {
        //�E�F�C�|�C���g���擾
        CollectWaypoints();

        Export();
    }

    // �^�O�t���I�u�W�F�N�g�����W����֐�
    private void CollectWaypoints()
    {
        //�O�̂���Clear���Ă���
        waypoints.Clear();
        //"WayPoint"�Ƃ����^�O���t�����I�u�W�F�N�g�����W����
        GameObject[] waypointObjects = GameObject.FindGameObjectsWithTag("WayPoint");
        foreach (var obj in waypointObjects)
        {
            //�z��ɒǉ�����
            waypoints.Add(obj);
        }

        UnityEngine.Debug.Log($"���W�����E�F�C�|�C���g�̐�: {waypoints.Count}");
    }

    private void Export()
    {
        //�o�͐���w�肷��
        string fileName = EditorUtility.SaveFilePanel("�G�̃E�F�C�|�C���g�f�[�^�̏o�͐��I��ł�������", "", SceneManager.GetActiveScene().name, "Way");

        //�o�͐悪�Ȃɂ��w�肳��Ȃ������Ƃ�(�L�����Z�����ꂽ�Ƃ�)
        if (fileName == "")
        {
            //���������ɏI����
            UnityEngine.Debug.LogWarning("�o�͂��܂���ł���");
            return;
        }
        //�t�@�C�����J��
        FileStream fs = File.Create(fileName);

        //�o�C�i���Ƃ��ď�������
        BinaryWriter binaryWriter = new BinaryWriter(fs);

        {
            binaryWriter.Write(waypoints.Count);
            foreach (var wp in waypoints)
            {
                // �����o���f�[�^�FID, ���W, �ڑ���ID, ����m��
                binaryWriter.Write(wp.name); // ID�Ƃ��Ė��O��ۑ�
                binaryWriter.Write(wp.transform.position.x);
                binaryWriter.Write(wp.transform.position.y);
                binaryWriter.Write(wp.transform.position.z);

                Waypoint waypoint = wp.GetComponent<Waypoint>();

                binaryWriter.Write(waypoint.connectedWaypoints.Count);
                for (int i = 0; i < waypoint.connectedWaypoints.Count; i++)
                {
                    binaryWriter.Write(waypoint.connectedWaypoints[i].name); // �ڑ���ID
                }
            }
        }

        UnityEngine.Debug.Log("Waypoint�f�[�^���G�N�X�|�[�g���܂���: " + fileName);
    }
}
