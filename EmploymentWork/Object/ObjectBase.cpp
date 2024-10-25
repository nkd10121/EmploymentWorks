#include "ObjectBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ObjectBase::ObjectBase(Priority priority, GameObjectTag tag):
	Collidable(priority,tag)
{
	rigidbody = std::make_shared<MyLib::Rigidbody>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ObjectBase::~ObjectBase()
{
	//���\�[�X�J��
	MV1DeleteModel(m_modelHandle);
}
