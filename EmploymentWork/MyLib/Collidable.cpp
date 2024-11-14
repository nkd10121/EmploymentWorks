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
	rigidbody = std::make_shared<MyLib::Rigidbody>();
}

/// <summary>
/// デストラクタ
/// </summary>
MyLib::Collidable::~Collidable()
{
}
void MyLib::Collidable::OnEntryPhysics()
{
	auto& physics = MyLib::Physics::GetInstance();
	physics.Entry(shared_from_this());
}

void MyLib::Collidable::OnExistPhysics()
{
	auto& physics = MyLib::Physics::GetInstance();
	physics.Exit(shared_from_this());
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
