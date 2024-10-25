#include "ObjectBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
ObjectBase::ObjectBase(Priority priority, GameObjectTag tag):
	Collidable(priority,tag)
{
	rigidbody = std::make_shared<MyLib::Rigidbody>();
}

/// <summary>
/// デストラクタ
/// </summary>
ObjectBase::~ObjectBase()
{
	//リソース開放
	MV1DeleteModel(m_modelHandle);
}
