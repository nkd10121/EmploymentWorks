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

void MyLib::Collidable::AddThroughTag(GameObjectTag tag)
{
	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(throughTags.begin(), throughTags.end(), tag) != throughTags.end());
	//リストの中に既に存在していたら警告を出す
	if (found)
	{
		assert(0 && "指定タグは既に追加されています");
	}
	else //存在していなかったら追加する
	{
		throughTags.emplace_back(tag);
	}
}

void MyLib::Collidable::RemoveThroughTag(GameObjectTag tag)
{
	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(throughTags.begin(), throughTags.end(), tag) != throughTags.end());
	//存在していなかったら警告を出す
	if (!found)
	{
		assert(0 && "指定タグは存在しません");
	}
	else //存在していたら削除する
	{
		throughTags.remove(tag);
	}
}

bool MyLib::Collidable::IsThroughTarget(const std::shared_ptr<Collidable> target) const
{
	//確認したい当たり判定のタグが無視するタグのリストに含まれているかどうか調べる
	bool found = (std::find(throughTags.begin(), throughTags.end(), target->GetTag()) != throughTags.end());
	return found;
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

	m_colliders.clear();
}

/// <summary>
/// 当たり判定を追加
/// </summary>
std::shared_ptr<MyLib::ColliderBase> MyLib::Collidable::AddCollider(const ColliderBase::Kind& kind, bool isTrigger, ColliderBase::CollisionTag collisionTag)
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

	Collide addInfo;
	addInfo.collide = add;
	addInfo.collideTag = collisionTag;

	//当たり判定を追加する
	m_colliders.emplace_back(addInfo);
	return add;
}

void MyLib::Collidable::DeleteCollider(Collide col)
{
	auto newEnd = remove(m_colliders.begin(), m_colliders.end(), col);
	m_colliders.erase(newEnd, m_colliders.end());
}

const MyLib::Collidable::Collide MyLib::Collidable::GetCollider(ColliderBase::CollisionTag collisionTag)const
{
	Collide ret;

	for (auto& col : m_colliders)
	{
		if (col.collideTag == collisionTag)
		{
			ret = col;
		}
	}

	return ret;
}
