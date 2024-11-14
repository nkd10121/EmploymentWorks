#pragma once
#include <list>

#include "EnemyBase.h"

/// <summary>
/// 敵の群れを管理するクラス
/// </summary>
class SwarmEnemy
{
public:
	SwarmEnemy();
	~SwarmEnemy();

	void Update();
	void Draw();

	void AddSwarm(std::list<EnemyBase> add, Vec3 center);

private:
	std::list<EnemyBase> m_swarm;	//群れの構成員
};

