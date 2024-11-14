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
	~SwarmEnemy();

	void Update();
	void Draw();

	void UpdateModelPos();

	/// <summary>
	/// 構成メンバーを追加
	/// </summary>
	/// <param name="add"></param>
	void AddSwarm(std::shared_ptr<EnemyBase> add);

	/// <summary>
	/// 構成メンバーが存在するかどうかを取得
	/// </summary>
	/// <returns>構成メンバーが存在するかどうか</returns>
	const bool GetIsExistMember()const { return m_isExistMember; }

private:
	std::list<std::shared_ptr<EnemyBase>> m_swarm;	//群れの構成員

	bool m_isExistMember;		//メンバーが存在するかどうか

	unsigned int m_memberColor;	//DEBUG用。誰がどの群れなのか見たい
};

