#include "EnemyNormal.h"

#include "LoadCSV.h"

namespace
{
	//デバッグ用のカプセル関係
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyNormal::EnemyNormal():
	EnemyBase()
{
	//MEMO:武器をゲームシーンに渡すために引数で持ってきた

	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCupsuleRadius;
	sphereCol->m_size = kCupsuleSize;
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyNormal::~EnemyNormal()
{
}

/// <summary>
/// 初期化
/// </summary>
void EnemyNormal::Init(std::shared_ptr<MyLib::Physics> physics)
{
	m_pPhysics = physics;

	Collidable::Init(physics);

	m_drawPos.x = static_cast<float>(GetRand(20) - 10);
	m_drawPos.y = 8.0f;
	m_drawPos.z = static_cast<float>(GetRand(20) - 10);

	//初期位置設定
	rigidbody->Init(true);
	rigidbody->SetPos(m_drawPos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus("EnemyNormal");
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	m_isExist = true;
}

/// <summary>
/// 終了
/// </summary>
void EnemyNormal::Finalize()
{
	Collidable::Finalize(m_pPhysics);
}

/// <summary>
/// 更新
/// </summary>
void EnemyNormal::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	if (m_status.hp <= 0)
	{
		m_isExist = false;
	}

	//速度を0にする(重力の影響を受けながら)
	auto prevVel = rigidbody->GetVelocity();
	rigidbody->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void EnemyNormal::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//FIX:Drawのなかで座標を変更しているのはどうなの？
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_drawPos = rigidbody->GetPos();

	//プレイヤー想定のカプセル
	VECTOR low = VGet(m_drawPos.x, m_drawPos.y - kCupsuleSize, m_drawPos.z);
	VECTOR high = VGet(m_drawPos.x, m_drawPos.y + kCupsuleSize, m_drawPos.z);
	DrawCapsule3D(low, high, kCupsuleRadius, kCupsuleDivNum, 0xffffff, 0xffffff, false);
}

