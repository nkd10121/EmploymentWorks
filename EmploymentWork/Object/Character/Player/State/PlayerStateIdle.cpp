#include "PlayerStateIdle.h"
#include "Input.h"
#include "Player.h"
#include "PlayerStateWalk.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStateIdle::PlayerStateIdle(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	m_nowState = StateKind::Idle;
}

/// <summary>
/// ������
/// </summary>
void PlayerStateIdle::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateIdle::Update()
{
	CheckPlayer();

	auto own = dynamic_cast<Player*>(m_pOwn.get());

	//���X�e�B�b�N�����͂���Ă�����State��Walk�ɂ���
	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
		Input::GetInstance().GetInputStick(false).second != 0.0f)
	{
		m_nextState = std::make_shared<PlayerStateWalk>(m_pOwn);
		auto state = std::dynamic_pointer_cast<PlayerStateWalk>(m_nextState);
		state->Init();
		return;
	}

	//�v���C���[�̑��x��0�ɂ���(�d�͂̉e�����󂯂Ȃ���)
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));
}

//int PlayerStateIdle::OnDamage(std::shared_ptr<MyLib::Collidable> collider)
//{
//	return 0;
//}
