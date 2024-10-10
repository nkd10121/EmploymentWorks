using UnityEngine;
/*�K�v�ȃ��C�u����*/
using UnityEditor;          //���j���[�Ȃǂ��o�����߂ɕK�v�Ȃ���
using System.IO;            //�o�͂��邽�߂ɕK�v
//using Unity.VisualScripting;

public class LocationOutputter : MonoBehaviour
{
    [MenuItem("���j���[/�z�u�f�[�^�o��")]
    public static void OutPutLocationData()
    {
        //�o�͐���w�肷��
        string fileName = EditorUtility.SaveFilePanel("�o�̓t�@�C��", "", "data", "loc");

        //�o�͐悪�Ȃɂ��w�肳��Ȃ������Ƃ�(�L�����Z�����ꂽ�Ƃ�)
        if (fileName == "")
        {
            //���������ɏI����
            Debug.LogWarning("�L�����Z������܂���");
            return;
        }

        //MEMO:�����܂ŗ�����o�͐悪���܂��Ă���
        Debug.Log(fileName);


        //�o�͂���֐����Ă�
        if (OutputData(fileName))
        {
            //�������Ȃ��I������m�F��ʂ��o��悤�ɂ���
            EditorUtility.DisplayDialog("�o�͂��܂���", $"{fileName}�ɏo�͂��܂���", "����");
        }
        else
        {
        }

    }

    private static void WriteVector(BinaryWriter bw,Vector3 vec)
    {
        bw.Write(vec.x);
        bw.Write(vec.y);
        bw.Write(vec.z);
    }

    private static bool WriteRecursiveData(BinaryWriter bw,GameObject gameObject,Vector3 offsetPos,Vector3 offsetRot,Vector3 mulScale)
    {
        if(gameObject == null)
        {
            return false;
        }

        if(gameObject.transform == null) 
        {
            return false;   
        }

        //���̃Q�[���I�u�W�F�N�g���q�̏ꍇ
        if(gameObject.transform.childCount == 0)
        {
            //���O�̏o��
            bw.Write(gameObject.name);  //�擪1�o�C�g�������񐔁A���Ƃ͕�����f�[�^
            //�^�O�̏o��
            bw.Write(gameObject.tag);  //�擪1�o�C�g�������񐔁A���Ƃ͕�����f�[�^

            //���W���̏o��
            WriteVector(bw, offsetPos + gameObject.transform.localPosition);                //���W
            WriteVector(bw, offsetRot + gameObject.transform.localRotation.eulerAngles);    //��](XYZ���̃I�C���[��])
            WriteVector(bw, Vector3.Scale(mulScale, gameObject.transform.localScale));      //�X�P�[�����O

            var d = offsetRot + gameObject.transform.localRotation.eulerAngles;
            Debug.Log(d);
        }
        //���̃Q�[���I�u�W�F�N�g���e�̏ꍇ
        else
        {
            for(int i = 0;i <  gameObject.transform.childCount;i++)
            {
                WriteRecursiveData(bw, gameObject.transform.GetChild(i).gameObject,
                    offsetPos + gameObject.transform.localPosition,
                    offsetRot + gameObject.transform.localRotation.eulerAngles,
                    Vector3.Scale(mulScale,gameObject.transform.localScale));
            }
        }

        return true;
    }

    private static int GetRecursizeLeafCount(GameObject gameObject)
    {
        int cnt = gameObject.transform.childCount;

        //�q�������Ȃ�=���[�t(���[)
        if(cnt == 0)
        {
            //�����������Ȃ�����1��Ԃ�
            return 1;
        }
        //�q���������ꍇ
        else
        {
            int sum = 0;
            for (int i = 0; i < cnt; i++)   //�J�E���g�͎q���ɂ�����
            {
                sum += GetRecursizeLeafCount(gameObject.transform.GetChild(i).gameObject);
            }
            return sum;
        }
    }

    private static int CalculateLeafCount(GameObject gameObject)
    {
        return GetRecursizeLeafCount(gameObject);
    }

    /// <summary>
    /// ���W�����o�͂���
    /// </summary>
    /// <param name="bw"></param>
    private static bool WriteTransformData(BinaryWriter bw)
    {
        //���ݑI�𒆂̃I�u�W�F�N�g������Ă���(�����炭�o�͂����������̃I�u�W�F�N�g����ꂽEmpty�I�u�W�F�N�g��I������)
        GameObject topObject = Selection.activeGameObject;
        if(topObject == null)
        {
            EditorUtility.DisplayDialog("", "�g�b�v���x���I�u�W�F�N�g��I�����Ă�������", "����");
            return false;
        }

        bw.Write(CalculateLeafCount(topObject));

        //�o�͂������I�u�W�F�N�g���o�͂���
        WriteRecursiveData(bw, topObject, new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f));

        return true;
    }


    /// <summary>
    /// �����o�͂���
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    private static bool OutputData(string fileName)
    {
        //�t�@�C�����J��
        FileStream fs = File.Create(fileName);

        //�o�C�i���Ƃ��ď�������
        BinaryWriter binaryWriter= new BinaryWriter(fs);
        if(!WriteTransformData(binaryWriter))
        {
            EditorUtility.DisplayDialog("�o�͂Ɏ��s���܂���", $"{fileName}�ɏo�͎��s���܂���", "����");
            return false;
        }

        //�������ݏI������Close����
        fs.Close();

        //�����܂ŗ�����Ȃɂ����Ȃ��I���ꂽ
        return true;
    }
}
