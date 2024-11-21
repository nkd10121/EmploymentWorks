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
void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& colider, int colIndex)
{

}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& colider, int colIndex)
{

}

const void EnemyBase::SetModelRotation(Vec3 rot) const
{
	MV1SetRotationXYZ(m_modelHandle, rot.ToVECTOR());
	MyLib::Collidable::Collide col = GetCollider(MyLib::ColliderBase::CollisionTag::Head);
	col.collide->localPos.SetFrontPos(m_playerPos);
}
