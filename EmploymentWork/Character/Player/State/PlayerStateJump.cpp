#include "PlayerStateJump.h"
#include "Input.h"
#include "Player.h"

#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"

#include "ModelManager.h"

namespace
{
	//ジャンプ力
	constexpr float kJumpPower = 1.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateJump::PlayerStateJump(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	m_jumpFrame(0)
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
	m_updateFunc = &PlayerStateJump::UpUpdate;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateJump::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckPlayer())	return;

	(this->*m_updateFunc)();

	//auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());


	//auto jumpAnimEndFrame = own->GetNowAnimEndFrame();

	//if (jumpAnimEndFrame * 0.72f < m_jumpFrame)
	//{
	//	//左スティックが入力されていなかったらStateをIdleにする
	//	if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
	//		Input::GetInstance().GetInputStick(false).second == 0.0f)
	//	{
	//		std::shared_ptr<PlayerStateIdle> pNext = std::make_shared<PlayerStateIdle>(m_pOwn.lock());
	//		pNext->Init();
	//		m_nextState = pNext;

	//		own->ChangeAnim(0);
	//		return;
	//	}

	//	//左スティックが入力されていたらStateをWalkにする
	//	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
	//		Input::GetInstance().GetInputStick(false).second != 0.0f)
	//	{
	//		std::shared_ptr<PlayerStateWalk> pNext = std::make_shared<PlayerStateWalk>(m_pOwn.lock());
	//		pNext->Init();
	//		m_nextState = pNext;

	//		m_pOwn.lock()->ChangeAnim(1, PlayerAnim::kWalkAnimSpeed);
	//		return;
	//	}
	//}

	//m_jumpFrame++;
}

void PlayerStateJump::UpUpdate()
{
	auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());
	if (own->GetNowAnimEndFrame() * 0.98f <= m_jumpFrame)
	{
		own->ChangeAnim(5);
		m_jumpFrame = 0;
		m_updateFunc = &PlayerStateJump::LoopUpdate;
	}

	m_jumpFrame++;
}

void PlayerStateJump::LoopUpdate()
{
	if (m_jumpFrame > 10)
	{
		auto stageModel = ModelManager::GetInstance().GetModelHandle("MOD_STAGECOLLISION");
		MV1SetScale(stageModel, VGet(0.1f, 0.1f, 0.1f));

		auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());
		auto pos = own->GetRigidbody()->GetPos();
		auto vel = own->GetRigidbody()->GetVelocity();
		auto modelBottomPos = pos;
		modelBottomPos.y -= own->GetCollisionSize();
		auto underPos = modelBottomPos;
		underPos.y -= (own->GetCollisionRadius() + own->GetCollisionSize()) * 2.0f * (0.4f - vel.y);

		auto hit = MV1CollCheck_Line(stageModel, -1, modelBottomPos.ToVECTOR(), underPos.ToVECTOR());

		if (hit.HitFlag)
		{
			own->ChangeAnim(6, 0.75f);
			m_jumpFrame = 0;
			m_updateFunc = &PlayerStateJump::DownUpdate;
		}

		DrawCapsule3D(modelBottomPos.ToVECTOR(), underPos.ToVECTOR(), 2, 5, 0xcccccc, 0xcccccc, false);

		MV1DeleteModel(stageModel);
	}
	else
	{
		m_jumpFrame++;
	}

}

void PlayerStateJump::DownUpdate()
{
	auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());

	if (m_jumpFrame >= own->GetNowAnimEndFrame() * 0.6f)
	{
		//左スティックが入力されていなかったらStateをIdleにする
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

			m_pOwn.lock()->ChangeAnim(1, PlayerAnim::kWalkAnimSpeed);
			return;
		}
	}
	m_jumpFrame++;
}
