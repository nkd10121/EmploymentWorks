#include "StateBase.h"
#include "CharacterBase.h"
#include <string>
#include <cassert>

/// <summary>
/// コンストラクタ
/// </summary>
StateBase::StateBase(std::shared_ptr<CharacterBase> own):
	m_pOwn(own),
	m_nowState(StateKind::Null)
{
}

#ifdef _DEBUG
/// <summary>
/// 現在のステートのデバッグ描画
/// </summary>
void StateBase::DebugDrawState(int x, int y)
{
	std::string own = "null";

	switch (m_pOwn.lock()->GetTag())
	{
	case GameObjectTag::Player:
		own = "Player";
		break;
	case GameObjectTag::Enemy:
		own = "Enemy";
		break;
	default:
		own = "null";
		break;
	}

	std::string state = "null";

	switch (m_nowState)
	{
	case StateBase::StateKind::Idle:
		state = "Idle";
		break;
	case StateBase::StateKind::Walk:
		state = "Walk";
		break;
	case StateBase::StateKind::Dash:
		state = "Dash";
		break;
	case StateBase::StateKind::Jump:
		state = "Jump";
		break;
	case StateBase::StateKind::Attack:
		state = "Attack";
		break;
	case StateBase::StateKind::OnHit:
		state = "OnHit";
		break;
	case StateBase::StateKind::Death:
		state = "Death";
		break;
	default:
		state = "null";
		break;
	}

	DrawFormatString(x, y, 0xffffff, "%s:%s", own.c_str(),state.c_str());
}
#endif

/// <summary>
/// 持ち主がプレイヤーかどうかを判断する
/// </summary>
bool StateBase::CheckPlayer()
{
	if (m_pOwn.lock()->GetTag() != GameObjectTag::Player)
	{
#ifdef _DEBUG
		assert(0 && "持ち主がプレイヤーではありません");
#endif
		return false;
	}

	return true;
}

bool StateBase::CheakEnemy()
{
	if (m_pOwn.lock()->GetTag() != GameObjectTag::Enemy)
	{
#ifdef _DEBUG
		assert(0 && "持ち主が敵ではありません");
#endif
		return false;
	}

	return true;
}
