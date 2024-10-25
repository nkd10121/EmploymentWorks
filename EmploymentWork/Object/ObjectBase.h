#pragma once
#include "DxLib.h"
#include "MyLib.h"
#include "Vec3.h"
#include <memory>

/// <summary>
/// �I�u�W�F�N�g�̊��N���X
/// </summary>
class ObjectBase : public MyLib::Collidable
{
public:
	/// <summary>
	///	�R���X�g���N�^
	/// </summary>
	/// <param name="priority">�D��x</param>
	/// <param name="tag">�^�O</param>
	ObjectBase(Priority priority, GameObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ObjectBase();

	///// <summary>
	///// ������
	///// </summary>
	//virtual void Init() {};
	///// <summary>
	///// �X�V
	///// </summary>
	//virtual void Update() {};
	///// <summary>
	///// �`��
	///// </summary>
	//virtual void Draw() {};

	/// <summary>
	/// ���f���̃��[�h��\������
	/// </summary>
	virtual void LoadModel() {};

protected:
	int m_modelHandle;	//���f���n���h��
};