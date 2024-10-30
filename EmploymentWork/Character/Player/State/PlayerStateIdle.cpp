#include "PlayerStateIdle.h"
#include "Input.h"
#include "Player.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateIdle::PlayerStateIdle(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	m_nowState = StateKind::Idle;
}

/// <summary>
/// 初期化
/// </summary>
void PlayerStateIdle::Init()
{
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateIdle::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckPlayer())	return;


	//左スティックが入力されていたらStateをWalkにする
	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
		Input::GetInstance().GetInputStick(false).second != 0.0f)
	{
		std::shared_ptr<PlayerStateWalk> pNext = std::make_shared<PlayerStateWalk>(m_pOwn.lock());
		pNext->Init();
		m_nextState = pNext;

		m_pOwn.lock()->ChangeAnim(1);
		return;
	}

	if (Input::GetInstance().IsTriggered("A"))
	{
		std::shared_ptr<PlayerStateJump> pNext = std::make_shared<PlayerStateJump>(m_pOwn.lock());
		pNext->Init();
		m_nextState = pNext;

		return;
	}

	auto own = dynamic_cast<Player*>(m_pOwn.lock().get());

	//プレイヤーの速度を0にする(重力の影響を受けながら)
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));
}

//int PlayerStateIdle::OnDamage(std::shared_ptr<MyLib::Collidable> collider)
//{
//	return 0;
//}
