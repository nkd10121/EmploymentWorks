#pragma once
#include "Vec3.h"
#include "CharacterBase.h"
#include "StateBase.h"

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player : public CharacterBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Player();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="physics">�����N���X�|�C���^</param>
	void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�̌��ݍ��W���擾����
	/// </summary>
	/// <returns>�v���C���[�̌��ݍ��W</returns>
	const MyLib::Vec3 GetPos()const { return rigidbody->GetPos(); }
	/// <summary>
	/// �J�����̕����x�N�g����ݒ肷��
	/// </summary>
	/// <param name="m_dir">�J�����̕����x�N�g��</param>
	void SetCameraAngle(MyLib::Vec3 m_dir) { m_cameraDirection = m_dir; }

	/// <summary>
	/// �J�����̕����x�N�g�����擾����
	/// </summary>
	/// <returns>�J�����̕����x�N�g��</returns>
	const MyLib::Vec3 GetCameraDirecton()const { return m_cameraDirection; }

	/// <summary>
	/// �����o���������s���I�u�W�F�N�g�ƏՓ˂����Ƃ�
	/// </summary>
	/// <param name="colider">�Փ˂����I�u�W�F�N�g</param>
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	/// <summary>
	/// �����o���������s��Ȃ��I�u�W�F�N�g�ƏՓ˂����Ƃ�
	/// </summary>
	/// <param name="colider">�Փ˂����I�u�W�F�N�g</param>
	void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

private:
	MyLib::Vec3 m_pos;				//�v���C���[�̕`����W
	MyLib::Vec3 temp_moveVec;		//���͏����m�F���邽�߂̃f�o�b�O�p
	MyLib::Vec3 m_cameraDirection;	//�J�����̕����x�N�g��
	MyLib::Vec3 m_rot;				//�p�x

	float m_cameraAngle;		//�J�����̊p�x
	float m_angle;				//�v���C���[�������Ă������

	std::shared_ptr<StateBase> m_pState;	//State�p�^�[��
};

