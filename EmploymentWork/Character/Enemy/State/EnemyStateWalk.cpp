#include "EnemyStateWalk.h"
#include "EnemyStateIdle.h"

#include "CharacterBase.h"
#include "EnemyBase.h"

#include "LoadCSV.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateWalk::EnemyStateWalk(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	temp_frame(0),
	m_playerPos()
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Walk;

	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "WALK_FORWARD"));
}

/// <summary>
/// 初期化
/// </summary>
void EnemyStateWalk::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void EnemyStateWalk::Update()
{
	//持ち主が敵かどうかをチェックする
	if (!CheakEnemy())	return;

	//ダウンキャスト
	auto own = std::dynamic_pointer_cast<EnemyBase>(m_pOwn.lock());

	auto playerPos = own->GetPlayerPos();
	auto moveVec = playerPos - own->GetRigidbody()->GetPos();

	if (moveVec.Length() <= 6.0f)
	{
		ChangeState(StateBase::StateKind::Attack);
	}

	moveVec = moveVec.Normalize() * own->GetMoveSpeed();

	//直前のY方向の移動速度と入力された移動速度を合わせて移動速度を決定する
	Vec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	Vec3 newVelocity = Vec3(moveVec.x, prevVelocity.y, moveVec.z);
	own->GetRigidbody()->SetVelocity(newVelocity);
}
