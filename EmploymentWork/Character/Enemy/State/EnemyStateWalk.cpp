#include "EnemyStateWalk.h"
#include "EnemyStateIdle.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateWalk::EnemyStateWalk(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	temp_frame(0)
{
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

	if (temp_frame >= 120)
	{
		std::shared_ptr<EnemyStateIdle> pNext = std::make_shared<EnemyStateIdle>(m_pOwn.lock());
		pNext->Init();
		m_nextState = pNext;

		return;
	}

	temp_frame++;
}
