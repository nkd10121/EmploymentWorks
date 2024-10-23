#pragma once
#include "DxLib.h"
#include "Vec3.h"
#include <memory>

/// <summary>
/// �J�����N���X
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

	/// <summary>
	/// �J�����������Ă�������x�N�g�����擾
	/// </summary>
	/// <returns>�J�����������Ă�������x�N�g��</returns>
	const MyLib::Vec3 GetDirection()const;

	/// <summary>
	/// �v���C���[�̍��W��ݒ肷��
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SetPlayerPos(MyLib::Vec3 playerPos) { m_playerPos = playerPos; }

private:
	float m_cameraAngleX;		//�����p�x
	float m_cameraAngleY;		//�����p�x
	MyLib::Vec3 m_cameraPos;	//�J�����̍��W
	MyLib::Vec3 m_aimPos;		//�����_
	MyLib::Vec3 m_playerPos;	//�v���C���[�̍��W
	float m_angleMoveScale;		//���_�ړ����x
	int m_lightHandle;			//���C�g�n���h��
};

