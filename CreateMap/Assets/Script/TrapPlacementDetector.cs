using UnityEngine;
using System.Collections.Generic;

/*必要なライブラリ*/
using UnityEditor;          //メニューなどを出すために必要なもの
using System.IO;            //出力するために必要

//Unityの現在のシーン名を取得したいため
using UnityEngine.SceneManagement; // コレ重要

public class TrapPlacementDetector : MonoBehaviour
{
    public float gridSize = 1.0f; // サンプリング間隔
    public float raycastLength = 10.0f; // Raycastの長さ
    public LayerMask terrainMask; // 地形レイヤーマスク

    [SerializeField] public GameObject minPos;
    [SerializeField] public GameObject maxPos;

    // 設置可能座標のリスト
    private List<Vector3> trapPositions = new List<Vector3>();

    void Start()
    {
        DetectTrapPlacementAreas();
        Debug.Log($"検出した設置可能座標: {trapPositions.Count} 箇所");

        OutPutTrapPositionData();
    }

    void DetectTrapPlacementAreas()
    {
        // 床、壁、天井の方向
        Vector3[] directions = { Vector3.down, Vector3.forward, Vector3.back, Vector3.left, Vector3.right, Vector3.up };

        // サンプリング範囲をループ
        for (float x = minPos.transform.position.x - gridSize / 2; x <= maxPos.transform.position.x; x += gridSize) // X範囲（適宜変更）
        {
            for (float y = minPos.transform.position.y - gridSize / 2; y <= maxPos.transform.position.y; y += gridSize) // Y範囲（高さ）
            {
                for (float z = minPos.transform.position.z - gridSize / 2; z <= maxPos.transform.position.z; z += gridSize) // Z範囲（適宜変更）
                {
                    Vector3 origin = new Vector3(x, y, z); // サンプリング開始点

                    // 各方向にRaycastを発射
                    foreach (Vector3 dir in directions)
                    {
                        Vector3 retHitPos = Vector3.zero;
                        if (CheckSurface(origin, dir, ref retHitPos))
                        {
                            trapPositions.Add(retHitPos); // 設置可能座標を追加
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
            // 法線で面の向きを判定（例: 床、壁、天井）
            Vector3 normal = hit.normal;

            // 条件: 床（下向き法線）、壁（横向き法線）、天井（上向き法線）
            if (IsFloor(normal) || IsWall(normal) || IsCeiling(normal))
            {
                // オブジェクトに特定のタグがある場合のみ有効
                if (hit.collider.CompareTag("TrapPlaceable"))
                {
                    ret = hit.point;
                    return true;
                }
            }
        }
        return false;
    }

    bool IsFloor(Vector3 normal) => Mathf.Abs(normal.y) > 0.9f; // 床: 法線がほぼ上向き
    bool IsWall(Vector3 normal) => Mathf.Abs(normal.y) < 0.1f; // 壁: 法線が水平
    bool IsCeiling(Vector3 normal) => normal.y < -0.9f;        // 天井: 法線が下向き

    // Gizmosで設置可能座標を表示
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
        //出力先を指定する
        string fileName = EditorUtility.SaveFilePanel("出力ファイル", "", SceneManager.GetActiveScene().name, "tLoc");

        //出力先がなにも指定されなかったとき(キャンセルされたとき)
        if (fileName == "")
        {
            //何もせずに終える
            Debug.LogWarning("出力しませんでした");
            return;
        }

        //ファイルを開く
        FileStream fs = File.Create(fileName);

        //バイナリとして書き込む
        BinaryWriter binaryWriter = new BinaryWriter(fs);

        binaryWriter.Write(trapPositions.Count);

        foreach (var pos in trapPositions)
        {
            WriteVector(binaryWriter, pos);
            Debug.Log(pos);
        }
        //書き込み終えたらCloseする
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