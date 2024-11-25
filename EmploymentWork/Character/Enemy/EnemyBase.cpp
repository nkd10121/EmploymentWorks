#include "EnemyBase.h"



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

void EnemyBase::SetPos(Vec3 pos)
{
	m_drawPos = pos;
}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& colider, int colIndex, const std::shared_ptr<Collide>& ownCol)
{

}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& colider, int colIndex, const std::shared_ptr<Collide>& ownCol)
{

}

const void EnemyBase::SetModelRotation(Vec3 rot) const
{
	MV1SetRotationXYZ(m_modelHandle, rot.ToVECTOR());
	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Head);
	if (col != nullptr)
	{
		col->collide->localPos.SetFrontPos(m_playerPos);
	}
}

const void EnemyBase::CreateAttackCollision()
{
	m_isAttack = true;

	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Attack);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = 4.0f;

	auto vec = m_playerPos - rigidbody->GetPos();
	auto attackPos = Vec3(vec.x, 0.0f,vec.z);

	sphereCol->localPos.localPos = attackPos;

	return void();
}

const void EnemyBase::DeleteAttackCollision()
{
	m_isAttack = false;

	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Attack);
	Collidable::DeleteRequestCollider(col);

	return void();
}

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
