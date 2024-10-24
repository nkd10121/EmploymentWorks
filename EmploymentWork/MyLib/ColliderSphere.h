#pragma once
#include "ColliderBase.h"

namespace MyLib
{
	/// <summary>
	/// 旧型の当たり判定
	/// </summary>
	class ColliderSphere : public ColliderBase
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="isTrigger">トリガーかどうか</param>
		ColliderSphere(bool isTrigger);

	public:
		float m_radius;	//半径
	};
}
