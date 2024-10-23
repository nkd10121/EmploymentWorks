#include <cassert> 
#include "DxLib.h"
#include "MyLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
MyLib::Collidable::Collidable(Priority priority, GameObjectTag tag) :
	priority(priority),
	tag(tag)
{
}

/// <summary>
/// デストラクタ
/// </summary>
MyLib::Collidable::~Collidable()
{
}

/// <summary>
/// 初期化(自身に当たり判定を追加)
/// </summary>
void MyLib::Collidable::Init(std::shared_ptr<MyLib::Physics> physics)
{
	physics->Entry(shared_from_this());	// 物理情報に自身を登録
}

/// <summary>
/// 終了(自身の当たり判定を削除)
/// </summary>
void MyLib::Collidable::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	physics->Exit(shared_from_this());	// 物理情報登録解除
}

/// <summary>
/// 当たり判定を追加
/// </summary>
std::shared_ptr<MyLib::ColliderBase> MyLib::Collidable::AddCollider(const ColliderBase::Kind& kind, bool isTrigger)
{
	std::shared_ptr<ColliderBase> add;

	//種類によって追加する当たり判定の派生先を変更する
	if (kind == ColliderBase::Kind::Sphere)
	{
		add = std::make_shared<ColliderSphere>(isTrigger);
	}
	else if(kind == ColliderBase::Kind::Cupsule)
	{
		add = std::make_shared<ColliderCupsule>(isTrigger);
	}
	else
	{
		assert(0 && "指定された種類の当たり判定を追加できませんでした");
	}

	//当たり判定を追加する
	m_colliders.emplace_back(add);

	return add;
}
