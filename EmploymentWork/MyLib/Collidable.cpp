#include <cassert> 
#include "DxLib.h"
#include "MyLib.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MyLib::Collidable::Collidable(Priority priority, GameObjectTag tag) :
	priority(priority),
	tag(tag)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MyLib::Collidable::~Collidable()
{
}

/// <summary>
/// ������(���g�ɓ����蔻���ǉ�)
/// </summary>
void MyLib::Collidable::Init(std::shared_ptr<MyLib::Physics> physics)
{
	physics->Entry(shared_from_this());	// �������Ɏ��g��o�^
}

/// <summary>
/// �I��(���g�̓����蔻����폜)
/// </summary>
void MyLib::Collidable::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	physics->Exit(shared_from_this());	// �������o�^����
}

/// <summary>
/// �����蔻���ǉ�
/// </summary>
std::shared_ptr<MyLib::ColliderBase> MyLib::Collidable::AddCollider(const ColliderBase::Kind& kind, bool isTrigger)
{
	std::shared_ptr<ColliderBase> add;

	//��ނɂ���Ēǉ����铖���蔻��̔h�����ύX����
	if (kind == ColliderBase::Kind::Sphere)
	{
		add = std::make_shared<ColliderSphere>(isTrigger);
	}
	else if(kind == ColliderBase::Kind::Cupsule)
	{
		add = std::make_shared<ColliderCupsule>(isTrigger);
	}
	else
	{
		assert(0 && "�w�肳�ꂽ��ނ̓����蔻���ǉ��ł��܂���ł���");
	}

	//�����蔻���ǉ�����
	m_colliders.emplace_back(add);

	return add;
}
