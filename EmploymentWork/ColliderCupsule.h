#pragma once
#include "ColliderBase.h"
#include "Vec3.h"

namespace MyLib
{
	class ColliderCupsule : public ColliderBase
	{
	public:
		//�R���X�g���N�^
		ColliderCupsule(bool isTrigger);

		//�傫��
		float m_size;
		//���a
		float m_radius;
	};
}


