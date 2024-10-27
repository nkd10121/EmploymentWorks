#include "Crystal.h"
#include <string>
#include "ModelManager.h"

#include "EnemyBase.h"

namespace
{
	//当たり判定の半径
	constexpr float kCollisionRadius = 6.0f;
	constexpr float kCollisionSize = 4.0f;

	//パス
	const std::string kCrystalPath = "data/model/stage/crystal.mv1";
	const std::string kCrystalStandPath = "data/model/stage/crystalStand.mv1";
}

Crystal::Crystal(int hp):
	ObjectBase(Collidable::Priority::Low,GameObjectTag::Crystal),
	m_pos(),
	m_hp(hp),
	m_isBreak(false),
	m_crystalStandHandle(-1),
	m_angle(0.0f)
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, true);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;
	sphereCol->m_size = kCollisionSize;
}

Crystal::~Crystal()
{
}

void Crystal::Init(std::shared_ptr<MyLib::Physics> physics)
{
	//当たり判定の初期化
	Collidable::Init(physics);
	//物理クラスの初期化
	rigidbody->Init();

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle(kCrystalPath);
	m_crystalStandHandle = ModelManager::GetInstance().GetModelHandle(kCrystalStandPath);

	MV1SetScale(m_modelHandle, VGet(0.2f, 0.2f, 0.2f));
	MV1SetScale(m_crystalStandHandle, VGet(0.2f, 0.2f, 0.2f));
}

void Crystal::Set(const Vec3& pos)
{
	m_pos = pos;

	m_pos.y -= 18.0f;

	MV1SetPosition(m_modelHandle, m_pos.ToVECTOR());
	MV1SetPosition(m_crystalStandHandle, m_pos.ToVECTOR());

	rigidbody->SetPos(pos);
}

void Crystal::Update()
{
	if (m_isBreak)	return;

	if (m_hp <= 0)
	{
		m_isBreak = true;
	}

	m_angle += 0.025f;
	auto posY = sinf(m_angle);

	MV1SetPosition(m_modelHandle, VECTOR(m_pos.x, m_pos.y + posY, m_pos.z));
	MV1SetRotationXYZ(m_modelHandle, VECTOR(0.0f, m_angle / 4, 0.0f));
}

void Crystal::Draw()
{
	MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_crystalStandHandle);
}

void Crystal::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
	//当たったオブジェクトが敵なら自身のHPを減らす
	if (colider->GetTag() == GameObjectTag::Enemy)
	{
		EnemyBase* col = dynamic_cast<EnemyBase*>(colider.get());
		col->End();

		m_hp--;

#ifdef _DEBUG
		printfDx("クリスタルにダメージが入りました。\n");
#endif
	}
}
