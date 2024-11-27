#include "SpikeTrap.h"

#include "ModelManager.h"
#include "LoadCSV.h"

namespace
{
	//当たり判定の円の半径
	constexpr float kCollisionRadius = 11.0f;

	//モデルサイズ
	constexpr float kModelScale = 0.08f;

	constexpr float kSpikeMoveSpeed = 0.8f;
}

SpikeTrap::SpikeTrap():
	TrapBase(),
	m_spikeModel(-1),
	m_attackCount(0),
	m_spikePos()
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true,MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;

	m_status = LoadCSV::GetInstance().LoadTrapStatus("Spike");
}


SpikeTrap::~SpikeTrap()
{
	MV1DeleteModel(m_spikeModel);
}


void SpikeTrap::Init(Vec3 pos)
{
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	rigidbody->SetPos(pos);
	rigidbody->SetNextPos(pos);

	m_spikePos = pos;
	m_spikePos.y -= 4.5f;
	m_spikePosInit = m_spikePos;

	//モデルのハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_SPIKEFRAME");
	MV1SetScale(m_modelHandle, VECTOR(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	m_spikeModel = ModelManager::GetInstance().GetModelHandle("M_SPIKE");
	MV1SetScale(m_spikeModel, VECTOR(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());

	//存在フラグをtrueにする
	m_isExist = true;
}


void SpikeTrap::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	if (m_isAttack)
	{
		//攻撃用当たり判定を生成する
		if (m_attackCount == 0)
		{
			//当たり判定の生成
			auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Attack);
			auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
			sphereCol->m_radius = kCollisionRadius;
		}

		if (m_attackCount < 30 && m_spikePos.y <= m_spikePosInit.y + 6.5f)
		{
			m_spikePos.y += kSpikeMoveSpeed;
			MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());
		}

		if (m_attackCount > 30 && m_spikePos.y >= m_spikePosInit.y)
		{
			m_spikePos.y -= kSpikeMoveSpeed;
			MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());
		}

		if (m_attackCount > 60)
		{
			m_isAttack = false;
			m_spikePos = m_spikePosInit;
			MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());
		}

		m_attackCount++;
	}
	else
	{
		m_attackCount = 0;
	}
}


void SpikeTrap::Draw()
{
	MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_spikeModel);
}