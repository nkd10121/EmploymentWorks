#include "EnemyStateWalk.h"
#include "EnemyStateIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateWalk::EnemyStateWalk(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	temp_frame(0)
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Walk;
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

	//2秒経過したら歩き状態にする
	if (temp_frame >= 120)
	{
		ChangeState(StateKind::Idle);
		return;
	}

	//フレーム数を更新する
	temp_frame++;
}
