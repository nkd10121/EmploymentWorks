#include "PlayerStateWalk.h"
#include "Input.h"
#include "Player.h"
#include "PlayerStateIdle.h"
#include "CharacterBase.h"

namespace
{
	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float kMaxSpeed = 0.2f;			//�v���C���[�̍ő呬�x
	constexpr float kAnalogRangeMin = 0.1f;		//�A�i���O�X�e�B�b�N�̓��͔���ŏ��͈�
	constexpr float kAnalogRangeMax = 0.8f;		//�A�i���O�X�e�B�b�N�̓��͔���ő�͈�
	constexpr float kAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStateWalk::PlayerStateWalk(std::shared_ptr<CharacterBase> own) :
	StateBase(own)
{
	m_nowState = StateKind::Walk;
}

/// <summary>
/// ������
/// </summary>
void PlayerStateWalk::Init()
{
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateWalk::Update()
{
	CheckPlayer();

	auto own = dynamic_cast<Player*>(m_pOwn.get());

	//���X�e�B�b�N�����͂���Ă�����State��Walk�ɂ���
	if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
		Input::GetInstance().GetInputStick(false).second == 0.0f)
	{
		m_nextState = std::make_shared<PlayerStateIdle>(m_pOwn);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

	//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	auto input = Input::GetInstance().GetInputStick(false);
	//�ړ�������ݒ肷��
	auto temp_moveVec = MyLib::Vec3(input.first,0.0f,-input.second);
	//�ړ��x�N�g���̒������擾����
	float len = temp_moveVec.Size();

	//�x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
	float rate = len / kAnalogInputMax;

	//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����(�f�b�h�]�[��)
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//���x������ł���̂ňړ��x�N�g���ɔ��f����
	temp_moveVec = temp_moveVec.Normalize();
	float speed = /*m_status.speed*/1.0f * rate;

	temp_moveVec = temp_moveVec * speed;

	//�v���C���[�̐��ʕ������v�Z���Đ��ʕ�������Ɉړ�����
	//�J�����̐��ʕ����x�N�g��
	MyLib::Vec3 front(own->GetCameraDirecton().x, 0.0f, own->GetCameraDirecton().z);
	//�����x�N�g��*�ړ���
	front = front * temp_moveVec.z;
	//�J�����̉E�����x�N�g��
	MyLib::Vec3 right(-own->GetCameraDirecton().z, 0.0f, own->GetCameraDirecton().x);
	//�����x�N�g��*�ړ���
	right = right * (-temp_moveVec.x);

	//�ړ��x�N�g���̐���
	temp_moveVec = front + right;
	temp_moveVec = temp_moveVec.Normalize() * speed;
	//�ړ�����
	//MV1SetPosition(m_modelHandle, m_collisionPos.ConvertToVECTOR());

	
	MyLib::Vec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLib::Vec3 newVelocity = MyLib::Vec3(temp_moveVec.x, prevVelocity.y, temp_moveVec.z);
	own->GetRigidbody()->SetVelocity(newVelocity);
}

//int PlayerStateWalk::OnDamage(std::shared_ptr<MyLib::Collidable> collider)
//{
//	return 0;
//}
