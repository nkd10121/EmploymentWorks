#include "Player.h"
#include "Input.h"
#include "MyLib.h"
#include "PlayerStateIdle.h"

namespace
{
	//�f�o�b�O�p�̃J�v�Z���֌W
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;

	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float kMaxSpeed = 0.2f;			//�v���C���[�̍ő呬�x
	constexpr float kAnalogRangeMin = 0.1f;		//�A�i���O�X�e�B�b�N�̓��͔���͈�
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�
}

Player::Player():
	CharacterBase(Collidable::Priority::Low, GameObjectTag::Player),
	m_pos(MyLib::Vec3(0.0f, kCupsuleRadius+ kCupsuleSize,0.0f)),
	temp_moveVec(),
	m_cameraDirection(),
	m_rot(),
	m_cameraAngle(0.0f),
	m_angle(0.0f)
{
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCupsuleRadius;
	sphereCol->m_size = kCupsuleSize;
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Init(physics);

	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->SetNextState(m_pState);
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(false);
	rigidbody.SetPos(m_pos);
	rigidbody.SetNextPos(rigidbody.GetPos());
}

void Player::Update()
{
	//�O�̃t���[����State���r���ĈႤState��������
	if (m_pState->GetNextState()->GetKind() != m_pState->GetKind())
	{
		//State��ύX����
		m_pState = m_pState->GetNextState();
		m_pState->SetNextState(m_pState);
	}

	//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	auto input = Input::GetInstance().GetInputStick(false);
	temp_moveVec = MyLib::Vec3(input.first,0.0f,-input.second);
	//�ړ�������ݒ肷��
	auto temp_moveVec = MyLib::Vec3(input.first, 0.0f, -input.second);
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
	MyLib::Vec3 front(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//�����x�N�g��*�ړ���
	front = front * temp_moveVec.z;
	//�J�����̉E�����x�N�g��
	MyLib::Vec3 right(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);
	//�����x�N�g��*�ړ���
	right = right * (-temp_moveVec.x);

	//�ړ��x�N�g���̐���
	temp_moveVec = front + right;
	temp_moveVec = temp_moveVec.Normalize() * speed;
	//�ړ�����
	//MV1SetPosition(m_modelHandle, m_collisionPos.ConvertToVECTOR());

	//�J�����̍��W����v���C���[����]������������v�Z����
	m_angle = -atan2f(m_cameraDirection.z, m_cameraDirection.x) - DX_PI_F / 2;
	m_rot = MyLib::Vec3(0.0f, m_angle, 0.0f);
	
	MyLib::Vec3 prevVelocity = rigidbody.GetVelocity();
	MyLib::Vec3 newVelocity = MyLib::Vec3(temp_moveVec.x, prevVelocity.y, temp_moveVec.z);
	rigidbody.SetVelocity(newVelocity);

	m_pState->Update();
}

void Player::Draw()
{
	//FIX:Draw�̂Ȃ��ō��W��ύX���Ă���̂͂ǂ��Ȃ́H
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_pos = rigidbody.GetPos();

	//�v���C���[�z��̃J�v�Z��
	VECTOR low = VGet(m_pos.x, m_pos.y - kCupsuleSize, m_pos.z);
	VECTOR high = VGet(m_pos.x, m_pos.y + kCupsuleSize, m_pos.z);
	DrawCapsule3D(low, high, kCupsuleRadius, kCupsuleDivNum, 0xffffff, 0xffffff, false);

#ifdef _DEBUG	//�f�o�b�O�`��
	//���͒l�̊m�F
	DrawFormatString(0, 16, 0xff0000, "���͒l�@: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
	//�v���C���[�̃X�e�[�g�p�^�[���̊m�F
	m_pState->DebugDrawState(0,32);
#endif
}

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
}
