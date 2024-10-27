#include "EnemyBase.h"

#include "Shot.h"
#include "SoundManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase():
	CharacterBase(Collidable::Priority::Middle, GameObjectTag::Enemy),
	kind(),
	m_isExist(false),
	m_drawPos(),
	m_hitObjectTag()
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{

}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
	//矢、トラップ
	m_hitObjectTag = colider->GetTag();
	if (m_hitObjectTag == GameObjectTag::PlayerShot)
	{
		{	
			//弾の攻撃力分HPを減らす
			Shot* col = dynamic_cast<Shot*>(colider.get());
			auto damage = col->GetAtk() - m_status.def;
			if (damage > 0)
			{
				m_status.hp -= damage;
			}

			SoundManager::GetInstance().PlaySE("seTest");

			//当たった弾の終了処理を呼ぶ
			col->End();

#ifdef _DEBUG
			printfDx("%d\n", m_status.hp);
#endif
		}
	}
}
