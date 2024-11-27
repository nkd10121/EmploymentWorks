#include "EnemyBase.h"

namespace
{
	//攻撃判定の半径
	constexpr float kAttackCollisionRadius = 4.0f;
	constexpr float kAttackCollisionDirection = 4.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase() :
	CharacterBase(Collidable::Priority::High, GameObjectTag::Enemy),
	kind(),
	m_isExist(false),
	m_isStartDeathAnimation(false),
	m_drawPos(),
	m_isSearchInPlayer(false),
	m_hitObjectTag()
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
	//モデルを削除する
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 座標を設定
/// </summary>
void EnemyBase::SetPos(Vec3 pos)
{
	m_drawPos = pos;
}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnCollideEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{

}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{

}

/// <summary>
/// モデルの向きを設定
/// </summary>
const void EnemyBase::SetModelRotation(Vec3 rot) const
{
	MV1SetRotationXYZ(m_modelHandle, rot.ToVECTOR());
	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Head);
	if (col != nullptr)
	{
		col->collide->localPos.SetFrontPos(m_playerPos);
	}
}

/// <summary>
/// 攻撃判定を作成
/// </summary>
const void EnemyBase::CreateAttackCollision(Vec3 vec)
{
	m_isAttack = true;

	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Attack);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kAttackCollisionRadius;

	auto attackPos = vec.Normalize() * kAttackCollisionDirection;
	attackPos = Vec3(attackPos.x, 0.0f, attackPos.z);

	sphereCol->localPos.localPos = attackPos;

	return void();
}

/// <summary>
/// 攻撃判定を削除
/// </summary>
const void EnemyBase::DeleteAttackCollision()
{
	m_isAttack = false;

	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Attack);
	if (col == nullptr)return;		//もし攻撃の判定を持っていなかったら早期リターン

	Collidable::DeleteRequestCollider(col);

	return void();
}

/// <summary>
/// 指定した当たり判定タグが存在するかチェック
/// </summary>
const bool EnemyBase::CheckIsExistCollisionTag(MyLib::ColliderBase::CollisionTag tag) const
{
	//存在するならtrue,しなかったらfalse
	for (auto& col : m_colliders)
	{
		if (col->collideTag == tag)
		{
			return true;
		}
	}
	return false;
}
