#include "EnemyStateIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateIdle::EnemyStateIdle(std::shared_ptr<CharacterBase> own) :
	StateBase(own)
{
	m_nowState = StateKind::Idle;
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
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheakEnemy())	return;
}
