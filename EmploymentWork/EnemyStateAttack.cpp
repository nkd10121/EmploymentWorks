#include "EnemyStateAttack.h"

#include "CharacterBase.h"
#include "EnemyBase.h"

#include "LoadCSV.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateAttack::EnemyStateAttack(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Attack;
	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "ATTACK"));
}

/// <summary>
/// 初期化
/// </summary>
void EnemyStateAttack::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void EnemyStateAttack::Update()
{
	auto own = std::dynamic_pointer_cast<EnemyBase>(m_pOwn.lock());

	//プレイヤーの速度を0にする(重力の影響を受けながら)
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));

	if (m_pOwn.lock()->GetNowAnimEndFrame() / 3 <= m_pOwn.lock()->GetAnimNowFrame())
	{
		ChangeState(StateKind::Walk);
	}
}
