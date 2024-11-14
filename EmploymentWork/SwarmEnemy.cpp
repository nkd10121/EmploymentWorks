#include "SwarmEnemy.h"

SwarmEnemy::SwarmEnemy(unsigned int color) :
	Collidable(Priority::Low,GameObjectTag::SwarmEnemy),
	m_isExistMember(false),
	m_memberColor(color)
{
}

SwarmEnemy::~SwarmEnemy()
{
	for (auto& enemy : m_swarm)
	{
		enemy->Finalize();
	}

	m_swarm.clear();
}

void SwarmEnemy::Update()
{
	//構成メンバーの更新
	for (auto& enemy : m_swarm)
	{
		enemy->Update();
	}

	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_swarm.begin(), m_swarm.end(), [](auto& v)
			{
				return v->GetIsExist() == false;
			});
		m_swarm.erase(it, m_swarm.end());
	}

	//もし、構成メンバーが1人もいなかったらメンバーが存在しないことにする
	if (m_swarm.size() == 0)	m_isExistMember = false;
}

void SwarmEnemy::Draw()
{
	//構成メンバーの描画
	for (auto& enemy : m_swarm)
	{
		enemy->Draw();

#ifdef _DEBUG
		auto pos = enemy->GetRigidbody()->GetPosVECTOR();
		pos.y += 6.0f;
		DrawSphere3D(pos,1,8,m_memberColor, m_memberColor,true);
#endif
	}
}

void SwarmEnemy::UpdateModelPos()
{
	//構成メンバーの描画
	for (auto& enemy : m_swarm)
	{
		enemy->UpdateModelPos();
	}
}

void SwarmEnemy::AddSwarm(std::shared_ptr<EnemyBase> add)
{
	//構成メンバーが存在しないことになっていたら存在するようにする
	if (!m_isExistMember)m_isExistMember = true;

	m_swarm.emplace_back(add);
}
