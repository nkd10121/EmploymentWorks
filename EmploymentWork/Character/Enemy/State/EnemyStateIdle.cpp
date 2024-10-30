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
		std::shared_ptr<EnemyStateWalk> pNext = std::make_shared<EnemyStateWalk>(m_pOwn.lock());
		pNext->Init();
		m_nextState = pNext;

		return;
	}

	temp_frame++;
}
