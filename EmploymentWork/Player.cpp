#include "Player.h"
#include "Input.h"
#include "MyLib.h"
#include "PlayerStateIdle.h"
#include "LoadCSV.h"
#include "HealPortion.h"

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

/// <summary>
/// �R���X�g���N�^
/// </summary>
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

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
}

/// <summary>
/// ������
/// </summary>
void Player::Init(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Init(physics);

	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->SetNextState(m_pState);
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody->Init(false);
	rigidbody->SetPos(m_pos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//�v���C���[�̃X�e�[�^�X�擾
	m_status = LoadCSV::GetInstance().LoadStatus("Player");
	//�ő�HP��ݒ肵�Ă���
	m_hpMax = m_status.hp;
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	//�O�̃t���[����State���r���ĈႤState��������
	if (m_pState->GetNextState()->GetKind() != m_pState->GetKind())
	{
		//State��ύX����
		m_pState = m_pState->GetNextState();
		m_pState->SetNextState(m_pState);
	}
	
	//�X�e�[�g�̍X�V
	m_pState->Update();
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	//FIX:Draw�̂Ȃ��ō��W��ύX���Ă���̂͂ǂ��Ȃ́H
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_pos = rigidbody->GetPos();

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

/// <summary>
/// �����o���������s���I�u�W�F�N�g�ƏՓ˂����Ƃ�
/// </summary>
void Player::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{
//#ifdef _DEBUG
//	std::string message = "�v���C���[��";
//#endif
//	auto tag = colider->GetTag();
//	switch (tag)
//	{
//	case GameObjectTag::Enemy:
//#ifdef _DEBUG
//		message += "�G";
//#endif
//		break;
//	case GameObjectTag::Portion:
//#ifdef _DEBUG
//		message += "�|�[�V����";
//#endif
//		break;
//	}
//#ifdef _DEBUG
//	message += "�Ɠ��������I\n";
//	printfDx(message.c_str());
//#endif
}

/// <summary>
/// �����o���������s��Ȃ��I�u�W�F�N�g�ƏՓ˂����Ƃ�
/// </summary>
void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
#ifdef _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = colider->GetTag();
	switch (tag)
	{
	case GameObjectTag::Enemy:
#ifdef _DEBUG
		message += "�G";
#endif
		break;
	case GameObjectTag::Portion:
#ifdef _DEBUG
		message += "�|�[�V����";
#endif
		//HP�������Ă���Ƃ��̂݉񕜏������s��
		//if (m_hpMax > m_status.hp)
		{
			//HP�𖞃^���܂ŉ񕜂�����
			m_status.hp = m_hpMax;

			//�|�[�V�������폜����
			HealPortion* col = dynamic_cast<HealPortion*>(colider.get());
			col->End();

			//�񕜃G�t�F�N�g�𐶐�����
			auto pos = rigidbody->GetPos();
			//EffectManager::GetInstance().CreateEffect("Heal", pos);

			//��SE�𗬂�
			//SoundManager::GetInstance().PlaySE("heal");
		}
		break;
	}
#ifdef _DEBUG
	message += "�Ɠ��������I\n";
	printfDx(message.c_str());
#endif
}
