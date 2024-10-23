#pragma once
#include <unordered_map>
#include "Vec3.h"

namespace MyLib
{
	/// <summary>
	/// 当たり判定のデータ基底クラス
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

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="kind">当たり判定種別</param>
		/// <param name="isTrigger">位置補正をするかどうか　false : しない, true : する</param>
		ColliderBase(Kind kind, bool isTrigger);
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~ColliderBase() {}

		/// <summary>
		/// 当たり判定の種別取得
		/// </summary>
		/// <returns>当たり判定の種別</returns>
		Kind GetKind() const { return kind; }

		/// <summary>
		/// トリガーかどうかを取得
		/// </summary>
		/// <returns>トリガーフラグ</returns>
		bool IsTrigger() const { return isTrigger; }

	private:
		Kind kind;		//当たり判定種別
		bool isTrigger;	//トリガーフラグ
	};
}
