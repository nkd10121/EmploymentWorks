#include "ColliderSphere.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MyLib::ColliderSphere::ColliderSphere(bool isTrigger) :
	ColliderBase(ColliderBase::Kind::Sphere, isTrigger),
	m_radius(0)
{
}
