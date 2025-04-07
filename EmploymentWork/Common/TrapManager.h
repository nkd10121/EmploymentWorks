#pragma once
#include "Vec3.h"
#include <memory>
#include <list>
#include <map>
#include <vector>
#include <string>

#include "TrapBase.h"

class TrapManager
{
public:
    struct Trap
    {
        Vec3 pos; // 位置
        Vec3 norm; // 法線ベクトル
        bool isPlaced; // 設置されているかどうか
        std::list<std::weak_ptr<Trap>> neighborTraps; // 隣接するトラップ
    };

    struct TrapInfo
    {
        int kind; // 設置種類
        std::string trapName; // 名前
        int modelHandle; // モデルID
        int imageHandle; // 画像ID
        int cost; // 設置コスト
    };

private:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    TrapManager();

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~TrapManager();

    static TrapManager* m_instance; // インスタンス

public:
    // コピーコンストラクタを禁止する
    TrapManager(const TrapManager&) = delete;
    TrapManager& operator=(const TrapManager&) = delete;
    TrapManager(TrapManager&&) = delete;
    TrapManager& operator=(const TrapManager&&) = delete;

    /// <summary>
    /// インスタンスを取得
    /// </summary>
    /// <returns>インスタンス</returns>
    static TrapManager& GetInstance()
    {
        if (!m_instance)
        {
            m_instance = new TrapManager;
        }

        return *m_instance;
    }

    /// <summary>
    /// 削除
    /// これをし忘れると普通にメモリリーク
    /// </summary>
    static void Destroy()
    {
        delete m_instance;
        m_instance = nullptr;
    }

private:
    /// <summary>
    /// 隣接するトラップをチェックする
    /// </summary>
    /// <param name="check">チェックするトラップのリスト</param>
    /// <returns>隣接するトラップがない場合はtrue</returns>
    const bool CheckNeighbor(std::list<std::weak_ptr<Trap>> check) const;

public:
    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw();

    /// <summary>
    /// プレビュー描画
    /// </summary>
    void PreviewDraw();

    /// <summary>
    /// ステージデータのロード
    /// </summary>
    /// <param name="stageName">ステージ名</param>
    void Load(const char* stageName);

    /// <summary>
    /// 初期設定
    /// </summary>
    /// <param name="point">初期ポイント</param>
    void SetUp(int point);

    /// <summary>
    /// 全消去
    /// </summary>
    void Clear();

    /// <summary>
    /// トラップポイントを追加する
    /// </summary>
    /// <param name="addPoint">追加するポイント</param>
    void AddTrapPoint(int addPoint);

    /// <summary>
    /// カメラ情報を設定する
    /// </summary>
    /// <param name="cameraPos">カメラの位置</param>
    /// <param name="dirVec">カメラの方向</param>
    const void SetCameraInfo(Vec3 cameraPos, Vec3 dirVec);

    /// <summary>
    /// スロットインデックスを設定する
    /// </summary>
    /// <param name="idx">スロットインデックス</param>
    const void SetSlotIdx(int idx);

    /// <summary>
    /// 準備フェーズかどうかを設定する
    /// </summary>
    /// <param name="isPrePhase">準備フェーズかどうか</param>
    const void SetIsPrePhase(bool isPrePhase) { m_isPrePhase = isPrePhase; }

private:
    std::list<std::shared_ptr<Trap>> m_trapPoss; // トラップのリスト
    std::list<std::shared_ptr<TrapBase>> m_traps; // トラップベースのリスト
    std::vector<std::string> m_trapNames; // トラップ名のリスト
    std::vector<std::shared_ptr<TrapBase>> m_previewTraps; // プレビュー用トラップのリスト

    float m_angle; // 罠を点滅させるためのアングル
    float m_transparency; // 罠の透明度

    int m_slotIdx; // プレイヤーのスロット番号
    int m_preSlotIdx; // プレイヤーのスロット番号のログ
    Vec3 m_cameraPos; // カメラ座標
    Vec3 m_cameraDir; // カメラの向いている方向

    int m_trapPoint; // 罠ポイント
    int m_targetTrapPoint; // 増減後罠ポイント

    int m_slotBgHandle; // スロット背景ハンドル
    int m_bgHandle; // 背景ハンドル
    int m_iconHandle; // アイコンハンドル

    bool m_isTextShake; // テキストが揺れているかどうか
    int m_textShakeFrame; // テキストが揺れるフレーム数

    bool m_isPrePhase; // プリフェーズかどうか

    float m_trapRotationAngle; // 罠の回転角度

    int m_attackEffectCreateCount; // 攻撃エフェクトの作成カウント
    std::string m_createEffectName; // 作成するエフェクト名

    std::shared_ptr<Trap> debugTrap; // デバッグ用トラップ
};