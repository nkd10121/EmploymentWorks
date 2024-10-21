#include "Player.h"
#include "Input.h"

namespace
{
	//�f�o�b�O�p�̃J�v�Z���֌W
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;
}

Player::Player():
	m_pos(),
	temp_moveVec()
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	auto input = Input::GetInstance().GetInputStick(false);
	temp_moveVec = MyLib::Vec3(input.first,0.0f,input.second);
	temp_moveVec = temp_moveVec.Normalize();
	//�ړ��x�N�g���̒������擾����
	float len = temp_moveVec.Size();
	
}

void Player::Draw()
{
	//�v���C���[�z��̃J�v�Z��
	VECTOR low = VGet(m_pos.x, m_pos.y - kCupsuleSize, m_pos.z);
	VECTOR high = VGet(m_pos.x, m_pos.y + kCupsuleSize, m_pos.z);
	DrawCapsule3D(low, high, kCupsuleRadius, kCupsuleDivNum, 0xffffff, 0xffffff, false);


#ifdef _DEBUG	//�f�o�b�O�`��
	//���͒l�̊m�F
	DrawFormatString(0, 16, 0xff0000, "���͒l�@: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
#endif
}
