#include "ColliderSphere.h"

MyLib::ColliderSphere::ColliderSphere(bool isTrigger) :
	ColliderBase(ColliderBase::Kind::Sphere, isTrigger),
	m_radius(0)
{
}
