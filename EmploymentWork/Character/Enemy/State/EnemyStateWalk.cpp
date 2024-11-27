#include "EnemyStateWalk.h"
#include "EnemyStateIdle.h"

#include "CharacterBase.h"
#include "EnemyBase.h"

#include "LoadCSV.h"

namespace
{
	//攻撃を開始するプレイヤーとの距離
	constexpr float kStartAttackLength = 6.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateWalk::EnemyStateWalk(std::shared_ptr<CharacterBase> own) :
	StateBase(own)
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Walk;
	//歩きアニメーションに変える
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

	//索敵範囲内にプレイヤーがいたら
	if (own->GetIsSearchInPlayer())
	{
		//自身からプレイヤーまでのベクトルを求める
		auto playerPos = own->GetPlayerPos();
		moveVec = playerPos - own->GetRigidbody()->GetPos();

		//距離が一定距離以下なら攻撃をする
		if (moveVec.Length() <= kStartAttackLength)
		{
			ChangeState(StateBase::StateKind::Attack);
			return;
		}

		//移動ベクトルを計算する
		moveVec = moveVec.Normalize() * own->GetMoveSpeed();
	}
	else
	{
		//索敵範囲内にプレイヤーがいなかったら待機状態に遷移する
		ChangeState(StateBase::StateKind::Idle);
		return;
	}

	//体を移動方向に向ける
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
