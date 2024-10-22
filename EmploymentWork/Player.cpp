#include "Player.h"
#include "Input.h"
#include "MyLib.h"

namespace
{
	//�f�o�b�O�p�̃J�v�Z���֌W
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;
}

Player::Player():
	CharacterBase(Collidable::Priority::Low, GameObjectTag::Player),
	m_pos(MyLib::Vec3(0.0f, kCupsuleRadius+ kCupsuleSize,0.0f)),
	temp_moveVec()
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

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(false);
	rigidbody.SetPos(m_pos);
	rigidbody.SetNextPos(rigidbody.GetPos());
}

void Player::Update()
{

	//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
	auto input = Input::GetInstance().GetInputStick(false);
	temp_moveVec = MyLib::Vec3(input.first,0.0f,-input.second);
	temp_moveVec = temp_moveVec.Normalize();
	//�ړ��x�N�g���̒������擾����
	float len = temp_moveVec.Size();
	
	MyLib::Vec3 prevVelocity = rigidbody.GetVelocity();
	MyLib::Vec3 newVelocity = MyLib::Vec3(temp_moveVec.x, prevVelocity.y, temp_moveVec.z);
	rigidbody.SetVelocity(newVelocity);
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
#endif
}

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
}
