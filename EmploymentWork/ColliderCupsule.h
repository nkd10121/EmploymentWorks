#pragma once
#include "ColliderBase.h"
#include "Vec3.h"

namespace MyLib
{
	class ColliderCupsule : public ColliderBase
	{
	public:
		//コンストラクタ
		ColliderCupsule(bool isTrigger);

		//大きさ
		float m_size;
		//半径
		float m_radius;
	};
}


