#include "PlayerStateIdle.h"
#include "Input.h"
#include "PlayerStateWalk.h"

PlayerStateIdle::PlayerStateIdle(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	m_nowState = StateKind::Idle;
}

void PlayerStateIdle::Init()
{
}

void PlayerStateIdle::Update()
{
	//左スティックが入力されていたらStateをWalkにする
	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
		Input::GetInstance().GetInputStick(false).second != 0.0f)
	{
		m_nextState = std::make_shared<PlayerStateWalk>(m_pOwn);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}
}

int PlayerStateIdle::OnDamage(std::shared_ptr<MyLib::Collidable> collider)
{
	return 0;
}
