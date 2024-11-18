#include "EnemyStateIdle.h"
#include "EnemyStateWalk.h"

#include "CharacterBase.h"
#include "EnemyBase.h"

#include "LoadCSV.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateIdle::EnemyStateIdle(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	temp_frame(0)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Idle;
	//アニメーションを待機にする
	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "IDLE"));
}

/// <summary>
/// 初期化
/// </summary>
void EnemyStateIdle::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void EnemyStateIdle::Update()
{
	//持ち主が敵かどうかをチェックする
	if (!CheakEnemy())	return;

	////2秒経過したら歩き状態にする
	//if (temp_frame >= 120)
	//{
	//	ChangeState(StateKind::Walk);
	//	return;
	//}

	////フレーム数を更新する
	//temp_frame++;

	auto own = std::dynamic_pointer_cast<EnemyBase>(m_pOwn.lock());

	//プレイヤーの速度を0にする(重力の影響を受けながら)
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));
}
