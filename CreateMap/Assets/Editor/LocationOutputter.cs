using UnityEngine;
/*必要なライブラリ*/
using UnityEditor;          //メニューなどを出すために必要なもの
using System.IO;            //出力するために必要
//using Unity.VisualScripting;

public class LocationOutputter : MonoBehaviour
{
    [MenuItem("メニュー/配置データ出力")]
    public static void OutPutLocationData()
    {
        //出力先を指定する
        string fileName = EditorUtility.SaveFilePanel("出力ファイル", "", "data", "loc");

        //出力先がなにも指定されなかったとき(キャンセルされたとき)
        if (fileName == "")
        {
            //何もせずに終える
            Debug.LogWarning("キャンセルされました");
            return;
        }

        //MEMO:ここまで来たら出力先が決まっている
        Debug.Log(fileName);


        //出力する関数を呼ぶ
        if (OutputData(fileName))
        {
            //何も問題なく終えたら確認画面が出るようにする
            EditorUtility.DisplayDialog("出力しました", $"{fileName}に出力しました", "閉じる");
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

        //そのゲームオブジェクトが子の場合
        if(gameObject.transform.childCount == 0)
        {
            //名前の出力
            bw.Write(gameObject.name);  //先頭1バイトが文字列数、あとは文字列データ
            //タグの出力
            bw.Write(gameObject.tag);  //先頭1バイトが文字列数、あとは文字列データ

            //座標情報の出力
            WriteVector(bw, offsetPos + gameObject.transform.localPosition);                //座標
            WriteVector(bw, offsetRot + gameObject.transform.localRotation.eulerAngles);    //回転(XYZ軸のオイラー回転)
            WriteVector(bw, Vector3.Scale(mulScale, gameObject.transform.localScale));      //スケーリング

            var d = offsetRot + gameObject.transform.localRotation.eulerAngles;
            Debug.Log(d);
        }
        //そのゲームオブジェクトが親の場合
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

        //子供がいない=リーフ(末端)
        if(cnt == 0)
        {
            //自分しかいないため1を返す
            return 1;
        }
        //子供がいた場合
        else
        {
            int sum = 0;
            for (int i = 0; i < cnt; i++)   //カウントは子供にさせる
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
    /// 座標情報を出力する
    /// </summary>
    /// <param name="bw"></param>
    private static bool WriteTransformData(BinaryWriter bw)
    {
        //現在選択中のオブジェクトを取ってくる(おそらく出力したい複数のオブジェクトを入れたEmptyオブジェクトを選択する)
        GameObject topObject = Selection.activeGameObject;
        if(topObject == null)
        {
            EditorUtility.DisplayDialog("", "トップレベルオブジェクトを選択してください", "閉じる");
            return false;
        }

        bw.Write(CalculateLeafCount(topObject));

        //出力したいオブジェクトを出力する
        WriteRecursiveData(bw, topObject, new Vector3(0.0f, 0.0f, 0.0f), new Vector3(0.0f, 0.0f, 0.0f), new Vector3(1.0f, 1.0f, 1.0f));

        return true;
    }


    /// <summary>
    /// 情報を出力する
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    private static bool OutputData(string fileName)
    {
        //ファイルを開く
        FileStream fs = File.Create(fileName);

        //バイナリとして書き込む
        BinaryWriter binaryWriter= new BinaryWriter(fs);
        if(!WriteTransformData(binaryWriter))
        {
            EditorUtility.DisplayDialog("出力に失敗しました", $"{fileName}に出力失敗しました", "閉じる");
            return false;
        }

        //書き込み終えたらCloseする
        fs.Close();

        //ここまで来たらなにも問題なく終えれた
        return true;
    }
}
