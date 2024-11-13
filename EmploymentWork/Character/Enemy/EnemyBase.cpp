#include "EnemyBase.h"

#include "Shot.h"
#include "SoundManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase():
	CharacterBase(Collidable::Priority::Middle, GameObjectTag::Enemy),
	kind(),
	m_isExist(false),
	m_isStartDeathAnimation(false),
	m_drawPos(),
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
void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{

}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = colider->GetTag();

	//当たったオブジェクトがプレイヤーが撃った弾なら
	if (m_hitObjectTag == GameObjectTag::PlayerShot)
	{
		{	
			//弾の攻撃力分自身のHPを減らす(防御力と調整しながら)
			Shot* col = dynamic_cast<Shot*>(colider.get());
			auto damage = col->GetAtk() - m_status.def;
			if (damage > 0)
			{
				m_status.hp -= damage;
			}

			//敵ヒットSEを流す
			SoundManager::GetInstance().PlaySE("S_ENEMYHIT");
			//敵ヒットエフェクトを出す
			EffectManager::GetInstance().CreateEffect("E_ENEMYHIT",rigidbody->GetPos());
			//当たった弾の終了処理を呼ぶ
			col->End();
#ifdef _DEBUG	//デバッグ描画

#endif
		}
	}
}
