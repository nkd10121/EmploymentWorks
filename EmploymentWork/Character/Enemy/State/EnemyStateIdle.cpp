#include "EnemyStateIdle.h"
#include "EnemyStateWalk.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateIdle::EnemyStateIdle(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	temp_frame(0)
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
	//持ち主が敵かどうかをチェックする
	if (!CheakEnemy())	return;

	if (temp_frame >= 120)
	{
		m_nextState = std::make_shared<EnemyStateWalk>(m_pOwn.lock());
		auto state = std::dynamic_pointer_cast<EnemyStateWalk>(m_nextState);
		state->Init();
		return;
	}

	temp_frame++;
}
