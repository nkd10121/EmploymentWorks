using UnityEngine;
using System.Collections.Generic;

/*必要なライブラリ*/
using UnityEditor;          //メニューなどを出すために必要なもの
using System.IO;            //出力するために必要

//Unityの現在のシーン名を取得したいため
using UnityEngine.SceneManagement; // コレ重要

public class ExportWaypoints : MonoBehaviour
{
    private List<GameObject> waypoints = new List<GameObject>();

    public void Start()
    {
        //ウェイポイントを取得
        CollectWaypoints();

        Export();
    }

    // タグ付きオブジェクトを収集する関数
    private void CollectWaypoints()
    {
        //念のためClearしておく
        waypoints.Clear();
        //"WayPoint"というタグが付いたオブジェクトを収集する
        GameObject[] waypointObjects = GameObject.FindGameObjectsWithTag("WayPoint");
        foreach (var obj in waypointObjects)
        {
            //配列に追加する
            waypoints.Add(obj);
        }

        UnityEngine.Debug.Log($"収集したウェイポイントの数: {waypoints.Count}");
    }

    private void Export()
    {
        //出力先を指定する
        string fileName = EditorUtility.SaveFilePanel("敵のウェイポイントデータの出力先を選んでください", "", SceneManager.GetActiveScene().name, "Way");

        //出力先がなにも指定されなかったとき(キャンセルされたとき)
        if (fileName == "")
        {
            //何もせずに終える
            UnityEngine.Debug.LogWarning("出力しませんでした");
            return;
        }
        //ファイルを開く
        FileStream fs = File.Create(fileName);

        //バイナリとして書き込む
        BinaryWriter binaryWriter = new BinaryWriter(fs);

        {
            binaryWriter.Write(waypoints.Count);
            foreach (var wp in waypoints)
            {
                // 書き出すデータ：ID, 座標, 接続先ID, 分岐確率
                binaryWriter.Write(wp.name); // IDとして名前を保存
                binaryWriter.Write(wp.transform.position.x);
                binaryWriter.Write(wp.transform.position.y);
                binaryWriter.Write(wp.transform.position.z);

                Waypoint waypoint = wp.GetComponent<Waypoint>();

                binaryWriter.Write(waypoint.connectedWaypoints.Count);
                for (int i = 0; i < waypoint.connectedWaypoints.Count; i++)
                {
                    binaryWriter.Write(waypoint.connectedWaypoints[i].name); // 接続先ID
                }
            }
        }

        UnityEngine.Debug.Log("Waypointデータをエクスポートしました: " + fileName);
    }
}
