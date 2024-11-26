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

/// <summary>
/// コンストラクタ
/// </summary>
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

/// <summary>
/// デストラクタ
/// </summary>
Crystal::~Crystal()
{
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_crystalStandHandle);
}

/// <summary>
/// 初期化
/// </summary>
void Crystal::Init()
{
	//当たり判定の初期化
	OnEntryPhysics();
	//物理クラスの初期化
	rigidbody->Init();

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_CRYSTAL");
	m_crystalStandHandle = ModelManager::GetInstance().GetModelHandle("M_CRYSTALSTAND");

	MV1SetScale(m_modelHandle, VGet(0.2f, 0.2f, 0.2f));
	MV1SetScale(m_crystalStandHandle, VGet(0.2f, 0.2f, 0.2f));
}

/// <summary>
/// 更新
/// </summary>
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

/// <summary>
/// 描画
/// </summary>
void Crystal::Draw()
{
	MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_crystalStandHandle);
}

/// <summary>
/// 生成座標を設定
/// </summary>
void Crystal::Set(const Vec3& pos)
{
	m_pos = pos;

	m_pos.y -= 18.0f;

	MV1SetPosition(m_modelHandle, m_pos.ToVECTOR());
	MV1SetPosition(m_crystalStandHandle, m_pos.ToVECTOR());

	rigidbody->SetPos(pos);
}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void Crystal::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトが敵なら自身のHPを減らす
	if (send->GetTag() == GameObjectTag::Enemy)
	{
		EnemyBase* col = dynamic_cast<EnemyBase*>(send.get());
		//col->End();

		m_hp--;

#ifdef _DEBUG	//デバッグ描画
		printf("クリスタルにダメージが入りました。\n");
#endif
	}
}
