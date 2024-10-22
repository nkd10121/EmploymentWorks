#pragma once
#include "DxLib.h"
#include "Vec3.h"
#include <memory>

/// <summary>
/// �J����
/// </summary>
class Camera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Camera();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�J�����������Ă�������x�N�g�����擾
	const MyLib::Vec3 GetDirection()const;

	//�v���C���[�̍��W��ݒ肷��
	void SetPlayerPos(MyLib::Vec3 playerPos) { m_playerPos = playerPos; }

private:
	//�����p�x
	float m_cameraAngleX;
	//�����p�x
	float m_cameraAngleY;
	//�J�����̍��W
	MyLib::Vec3 m_cameraPos;
	//�����_
	MyLib::Vec3 m_aimPos;
	//�v���C���[�̍��W
	MyLib::Vec3 m_playerPos;
	//���_�ړ����x
	float m_angleMoveScale;
	//���C�g�n���h��
	int m_lightHandle;
};

