#include "ColliderCupsule.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MyLib::ColliderCupsule::ColliderCupsule(bool isTrigger) :
	ColliderBase(ColliderBase::Kind::Cupsule, isTrigger),
	m_radius(0.0f),
	m_size(0.0f)
{
}