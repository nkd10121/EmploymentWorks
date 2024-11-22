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
	auto own = std::dynamic_pointer_cast<EnemyBase>(m_pOwn.lock());
	own->CreateAttackCollision();
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

	auto frame = m_pOwn.lock()->GetAnimNowFrame();
	//アニメーション上で攻撃が一回終了した時
	if (static_cast<int>(frame) % static_cast <int>(40.0f) == 0 && frame >= 1)
	{
		own->DeleteAttackCollision();

		//索敵範囲内にプレイヤーがいて
		if (own->GetIsSearchInPlayer())
		{
			auto playerPos = own->GetPlayerPos();
			auto moveVec = playerPos - own->GetRigidbody()->GetPos();

			//プレイヤーとの距離が一定距離以下の時は攻撃
			if (moveVec.Length() <= 6.0f)
			{
				//atan2を使用して向いている角度を取得
				auto angle = atan2(moveVec.x, moveVec.z);
				auto rotation = VGet(0.0f, angle + DX_PI_F, 0.0f);
				//移動方向に体を回転させる
				own->SetModelRotation(rotation);

				own->CreateAttackCollision();
				ChangeState(StateBase::StateKind::Attack);
				return;
			}
			//離れていたら歩きに遷移する
			else
			{
				ChangeState(StateKind::Walk);
				return;
			}
		}
		//索敵範囲内にプレイヤーがいなかったら待機にする
		else
		{
			ChangeState(StateKind::Idle);
			return;
		}
	}
}
