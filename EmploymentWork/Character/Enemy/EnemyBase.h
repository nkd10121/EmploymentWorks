#pragma once
#include "CharacterBase.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	//種類
	enum EnemyKind
	{
		EnemyNormal,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBase();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBase();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Init() {};
	/// <summary>
	/// 終了
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Finalize() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

	void SetPos(Vec3 pos);

	/// <summary>
	/// 終了(他から呼ぶ)
	/// </summary>
	void End() { m_isExist = false; }

	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider, int colIndex)override;
	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnTriggerEnter(const std::shared_ptr<Collidable>& colider, int colIndex)override;

	/// <summary>
	/// 索敵判定の作成
	/// </summary>
	virtual void CreateSearchCollision() {};

	virtual void DeleteSearchCollision() {};

	/// <summary>
	/// 3Dモデルの座標更新
	/// </summary>
	virtual void UpdateModelPos() {};

	/// <summary>
	/// 存在フラグを取得
	/// </summary>
	/// <returns>存在フラグ</returns>
	const bool GetIsExist()const { return m_isExist; }

	/// <summary>
	/// 死亡アニメーションを開始しているかどうかを取得
	/// </summary>
	/// <returns></returns>
	const bool GetIsStartDeathAnim()const { return m_isStartDeathAnimation; }


	const bool GetIsSearchInPlayer()const { return m_isSearchInPlayer; }
	const Vec3 GetPlayerPos()const { return m_playerPos; }

	virtual const float GetSearchCollisionRadius()const = 0;

protected:
	EnemyKind kind;		//種類
	bool m_isExist;		//存在フラグ
	bool m_isStartDeathAnimation;	//死亡アニメーションを開始するか

	Vec3 m_drawPos;		//描画座標

	bool m_isSearchInPlayer;		//索敵範囲内にプレイヤーがいるか
	Vec3 m_playerPos;				//プレイヤーの座標

	GameObjectTag m_hitObjectTag;	//あったオブジェクトタグ
};