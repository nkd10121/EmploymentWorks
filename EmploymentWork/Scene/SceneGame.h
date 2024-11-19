#pragma once
#include "SceneBase.h"
#include "Camera.h"
#include "MyLib.h"
//テスト用
#include <list>

//プロトタイプ宣言
class Player;		//プレイヤー
class ObjectBase;	//回復ポーション
class SwarmEnemy;	//敵
class Crystal;		//クリスタル

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGame();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneGame();

	/// <summary>
	/// //リソースのロード開始
	/// </summary>
	virtual void StartLoad() override;
	/// <summary>
	/// リソースのロードが終了したかどうか
	/// </summary>
	/// <returns>true : 終了済み, false : 終了していない</returns>
	virtual bool IsLoaded() const override;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;
	/// <summary>
	/// 終了
	/// </summary>
	virtual void End() override;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// オブジェクトを追加する
	/// </summary>
	/// <param name="pAddObject"></param>
	void AddObject(std::shared_ptr<ObjectBase> pAddObject);

	//状態遷移のためのメンバ関数
	using UpdateFunc_t = void (SceneGame::*)();
	using DrawFunc_t = void (SceneGame::*)();
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;
	/*状態関数*/
	/// <summary>
	/// ゲームの更新
	/// </summary>
	void UpdateGame();
	/// <summary>
	/// トラップ選択画面の更新
	/// </summary>
	void UpdateTrapSelect();

	/// <summary>
	/// 通常描画
	/// </summary>
	void DrawNormal();
	/// <summary>
	/// トラップ選択画面の描画
	/// </summary>
	void DrawTrapSelect();

private:
	std::shared_ptr<Player> m_pPlayer;			//プレイヤーポインタ
	std::shared_ptr<Camera> m_pCamera;			//カメラポインタ
	std::shared_ptr<Crystal> m_pCrystal;			//カメラポインタ
	std::shared_ptr<MyLib::Physics> m_pPhysics;	//物理クラスポインタ
	std::list<std::shared_ptr<ObjectBase>> m_pObjects;	//オブジェクトポインタの配列
	std::list<std::shared_ptr<SwarmEnemy>> m_pEnemies;	//敵ポインタの配列

	int m_stageModel;	//モデルハンドル(仮)

private:
	int m_nowCursor;
};

