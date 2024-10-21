#pragma once
#include <unordered_map>
#include "Vec3.h"

namespace MyLib
{
	/// <summary>
	/// 当たり判定のデータ基底
	/// </summary>
	class ColliderBase abstract
	{
	public:
		// 当たり判定種別
		enum class Kind
		{
			Sphere,		//球
			Cupsule,	//カプセル
		};

		// コンストラクタ
		ColliderBase(Kind kind, bool isTrigger)
		{
			this->kind = kind;
			this->isTrigger = isTrigger;
		}

		virtual ~ColliderBase() {}

		// 当たり判定種別取得
		Kind GetKind() const { return kind; }

		// トリガーかどうか
		bool IsTrigger() const { return isTrigger; }

	private:
		Kind	kind;
		bool	isTrigger;
	};
}
