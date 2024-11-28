#pragma once
#include "MyLib.h"
#include <memory>

//プロトタイプ宣言
class Player;		//プレイヤー
class Camera;		//カメラ
class ObjectBase;	//回復ポーション
class SwarmEnemy;	//敵
class Crystal;		//クリスタル

class GameManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// オブジェクトを追加する
	/// </summary>
	/// <param name="pAddObject"></param>
	void AddObject(std::shared_ptr<ObjectBase> pAddObject);

private:
	std::shared_ptr<Player> m_pPlayer;			//プレイヤーポインタ
	std::shared_ptr<Camera> m_pCamera;			//カメラポインタ
	std::shared_ptr<Crystal> m_pCrystal;		//カメラポインタ
	std::shared_ptr<MyLib::Physics> m_pPhysics;	//物理クラスポインタ
	std::list<std::shared_ptr<ObjectBase>> m_pObjects;	//オブジェクトポインタの配列
	std::list<std::shared_ptr<SwarmEnemy>> m_pEnemies;	//敵ポインタの配列

	int m_stageModel;	//モデルハンドル(仮)
};