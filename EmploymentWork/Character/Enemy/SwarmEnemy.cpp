#include "SwarmEnemy.h"

SwarmEnemy::SwarmEnemy(unsigned int color) :
	Collidable(Priority::Low, GameObjectTag::SwarmEnemy),
	m_isExistMember(false),
	m_swarmCenterPos(),
	m_swarmRadius(0.0f),
	m_isInPlayer(false),
	m_memberColor(color)
{
	//物理データの初期化
	rigidbody->Init();
}

SwarmEnemy::~SwarmEnemy()
{

}

void SwarmEnemy::Finalize()
{
	for (auto& enemy : m_swarm)
	{
		enemy->Finalize();
	}

	m_swarm.clear();
}

void SwarmEnemy::Update()
{
	std::list<Vec3> pos;
	//構成メンバーの更新
	for (auto& enemy : m_swarm)
	{
		enemy->Update();
		pos.emplace_back(enemy->GetRigidbody()->GetPos());
	}

	Vec3 addPos;
	for (auto& p : pos)
	{
		addPos += p;
	}
	//中心座標の決定
	m_swarmCenterPos = addPos / m_swarm.size();
	rigidbody->SetPos(m_swarmCenterPos);

	float maxLength = 0.0f;

	for (auto& p : pos)
	{
		auto length = (p - m_swarmCenterPos).Length();

		if (maxLength < length)
		{
			maxLength = length;
		}
	}

	if (!m_isInPlayer)
	{
		if (m_swarmRadius != maxLength)
		{
			m_swarmRadius = maxLength;
			auto cupsuleCol = dynamic_cast<MyLib::ColliderSphere*>(Collidable::GetCollider(MyLib::ColliderBase::CollisionTag::Search).get());			//キャスト
			cupsuleCol->SetRadius(m_swarmRadius);
		}
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
		DrawSphere3D(pos, 1, 8, m_memberColor, m_memberColor, true);


		DrawSphere3D(m_swarmCenterPos.ToVECTOR(), 4, 12, m_memberColor, m_memberColor, true);
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

void SwarmEnemy::SetUp()
{
	//群れ全体の中心座標と半径を求めて当たり判定を生成する
	Vec3 addPos;
	for (auto& enemy : m_swarm)
	{
		addPos += enemy->GetRigidbody()->GetPos();
	}
	//中心座標の決定
	m_swarmCenterPos = addPos / m_swarm.size();

	float maxLength = 0.0f;

	for (auto& enemy : m_swarm)
	{
		auto length = (enemy->GetRigidbody()->GetPos() - m_swarmCenterPos).Length();

		if (maxLength < length)
		{
			maxLength = length;
		}
	}
	m_swarmRadius = maxLength;

	{
		//当たり判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true);	//追加
		auto cupsuleCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());			//キャスト
		cupsuleCol->m_radius = m_swarmRadius;		//カプセルの半径

		Collidable::OnEntryPhysics();

		rigidbody->SetPos(m_swarmCenterPos);
		rigidbody->SetNextPos(rigidbody->GetPos());
	}
}

void SwarmEnemy::AddSwarm(std::shared_ptr<EnemyBase> add)
{
	//構成メンバーが存在しないことになっていたら存在するようにする
	if (!m_isExistMember)m_isExistMember = true;

	m_swarm.emplace_back(add);
}

void SwarmEnemy::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
	auto tag = colider->GetTag();
	if (tag == GameObjectTag::Player)
	{
		//当たり判定を消す
		Collidable::OnExistPhysics();

		//プレイヤーがいる判定にする
		m_isInPlayer = true;

		for (auto& enemy : m_swarm)
		{
			enemy->CreateSearchCollision();
		}
	}
}