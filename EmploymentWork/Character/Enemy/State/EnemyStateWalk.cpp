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
	Vec3 moveVec;

	if (own->GetIsSearchInPlayer())
	{
		auto playerPos = own->GetPlayerPos();
		moveVec = playerPos - own->GetRigidbody()->GetPos();

		if (moveVec.Length() <= 6.0f)
		{
			ChangeState(StateBase::StateKind::Attack);
			return;
		}

		moveVec = moveVec.Normalize() * own->GetMoveSpeed();
	}

	auto pos = m_pOwn.lock()->GetRigidbody()->GetPos();
	//atan2を使用して向いている角度を取得
	auto angle = atan2(moveVec.x, moveVec.z);
	auto rotation = VGet(0.0f, angle + DX_PI_F, 0.0f);
	//移動方向に体を回転させる
	own->SetModelRotation(rotation);


	//直前のY方向の移動速度と入力された移動速度を合わせて移動速度を決定する
	Vec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	Vec3 newVelocity = Vec3(moveVec.x, prevVelocity.y, moveVec.z);
	own->GetRigidbody()->SetVelocity(newVelocity);
}
