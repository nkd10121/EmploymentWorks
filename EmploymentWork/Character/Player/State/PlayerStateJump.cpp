#include "PlayerStateJump.h"
#include "Input.h"
#include "Player.h"

#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"

namespace
{
	//ジャンプ力
	constexpr float kJumpPower = 0.8f;
}

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateJump::PlayerStateJump(std::shared_ptr<CharacterBase> own):
	StateBase(own),
	m_jumpCount(0)
{
	m_nowState = StateKind::Jump;

	auto vel = own->GetRigidbody()->GetVelocity();
	vel.y += kJumpPower;
	own->GetRigidbody()->SetVelocity(vel);
}

/// <summary>
/// 初期化
/// </summary>
void PlayerStateJump::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateJump::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckPlayer())	return;

	auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());

	if (m_jumpCount > 2)
	{
		//左スティックが入力されていたらStateをWalkにする
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			std::shared_ptr<PlayerStateIdle> pNext = std::make_shared<PlayerStateIdle>(m_pOwn.lock());
			pNext->Init();
			m_nextState = pNext;

			own->ChangeAnim(0);
			return;
		}

		//左スティックが入力されていたらStateをWalkにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			std::shared_ptr<PlayerStateWalk> pNext = std::make_shared<PlayerStateWalk>(m_pOwn.lock());
			pNext->Init();
			m_nextState = pNext;

			own->ChangeAnim(1);
			return;
		}
	}

	m_jumpCount++;

	if (own->GetIsAnimationEnd())
	{

	}

}
