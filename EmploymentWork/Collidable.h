#pragma once
#include "GameObjectTag.h"
#include <List>
#include <memory>

namespace MyLib
{
	class Rigidbody;
	class Physics;
	class ColliderBase;

	/// <summary>
	/// 当たり判定クラス
	/// </summary>
	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:

		// 位置補正の優先度の判別に使う
		enum class Priority : int
		{
			Low,		// 低
			Middle,		// 中
			High,		// 高
			Static,		// 動かない（最高）
		};

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="priority">優先度</param>
		/// <param name="tag">タグ</param>
		Collidable(Priority priority, GameObjectTag tag);
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Collidable();
		/// <summary>
		/// 初期化(自身に当たり判定を追加)
		/// </summary>
		/// <param name="physics">物理クラスポインタ</param>
		virtual void Init(std::shared_ptr<MyLib::Physics> physics);
		/// <summary>
		/// 終了(自身の当たり判定を削除)
		/// </summary>
		/// <param name="physics">物理クラスポインタ</param>
		virtual void Finalize(std::shared_ptr<MyLib::Physics> physics);

		//当たり判定関数
		virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnCollideStay(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnCollideExit(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerEnter(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerStay(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerExit(const std::shared_ptr<Collidable>& colider) {}

		/// <summary>
		/// タグを取得
		/// </summary>
		/// <returns>タグ</returns>
		GameObjectTag GetTag() const { return tag; }
		/// <summary>
		/// 優先度を取得
		/// </summary>
		/// <returns>優先度</returns>
		Priority GetPriority() const { return priority; }

	protected:
		/// <summary>
		/// 当たり判定を追加
		/// </summary>
		/// <param name="kind">当たり判定の種類</param>
		/// <param name="isTrigger">押し出し判定をするかどうか</param>
		/// <returns>自身</returns>
		std::shared_ptr<ColliderBase> AddCollider(const ColliderBase::Kind& kind, bool isTrigger);

	protected:
		// 物理データ
		std::shared_ptr<Rigidbody> rigidbody;
		//当たり判定情報
		std::list<std::shared_ptr<ColliderBase>> m_colliders;

	private:
		//タグ
		GameObjectTag tag;
		//優先度
		Priority priority;
	};
}