#pragma once
#include "ColliderBase.h"

namespace MyLib
{
	class ColliderSphere : public ColliderBase
	{
	public:
		//�R���X�g���N�^
		ColliderSphere(bool isTrigger);

		//���a
		float m_radius;
	};
}
