#pragma once
#include "Vec3.h"
#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLib::Physics> physics);
	void Update();
	void Draw();

	const MyLib::Vec3 GetPos()const { return rigidbody.GetPos(); }
	void SetCameraAngle(MyLib::Vec3 m_dir) { m_cameraDirection = m_dir; }

	// �Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

private:
	MyLib::Vec3 m_pos;				//�v���C���[�̕`����W
	MyLib::Vec3 temp_moveVec;		//���͏����m�F���邽�߂̃f�o�b�O�p
	MyLib::Vec3 m_cameraDirection;	//�J�����̌���
	MyLib::Vec3 m_rot;				//�p�x

	float  m_cameraAngle;		//�J�����̊p�x
	float m_angle;				//�v���C���[�������Ă������

};

