#pragma once
#include "ColliderBase.h"

namespace MyLib
{
	class ColliderSphere : public ColliderBase
	{
	public:
		//コンストラクタ
		ColliderSphere(bool isTrigger);

		//半径
		float m_radius;
	};
}
