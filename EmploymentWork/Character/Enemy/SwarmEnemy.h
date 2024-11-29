#pragma once
#include <list>

#include "EnemyBase.h"

/// <summary>
/// 敵の群れを管理するクラス
/// </summary>
class SwarmEnemy : public MyLib::Collidable
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="color">デバッグ用色</param>
	SwarmEnemy(unsigned int color);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SwarmEnemy();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// 群れメンバーのモデル座標を更新
	/// </summary>
	void UpdateModelPos();

	/// <summary>
	/// 群れメンバーを追加しきった後に呼ぶ関数
	/// </summary>
	void SetUp();

	/// <summary>
	/// 構成メンバーを追加
	/// </summary>
	/// <param name="add"></param>
	void AddSwarm(std::shared_ptr<EnemyBase> add);

	/// <summary>
	/// メンバーの人数を取得
	/// </summary>
	/// <returns></returns>
	const int CheckMemberNum()const;

	/// <summary>
	/// 構成メンバーが存在するかどうかを取得
	/// </summary>
	/// <returns>構成メンバーが存在するかどうか</returns>
	const bool GetIsExistMember()const { return m_isExistMember; }
	/// <summary>
	/// 押し出し処理を行わないオブジェクトと衝突したとき
	/// </summary>
	void OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)override;
	/// <summary>
	/// 押し出し処理を行わないオブジェクトと衝突しなくなった時
	/// </summary>
	void OnTriggerExit(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)override;

	const std::list<int> GetModelHandles()const;
private:
	std::list<std::shared_ptr<EnemyBase>> m_swarm;	//群れの構成員
	bool m_isExistMember;		//メンバーが存在するかどうか

	Vec3 m_swarmCenterPos;	//群れ全体の中心座標
	float m_swarmRadius;	//群れ全体の半径
	float m_maxSearchCollisionRadius;		//群れの個体の中で一番大きい索敵範囲

	bool m_isInPlayer;		//プレイヤーが当たり判定内に入っているかどうか

	unsigned int m_memberColor;	//DEBUG用。誰がどの群れなのか見たい
};

