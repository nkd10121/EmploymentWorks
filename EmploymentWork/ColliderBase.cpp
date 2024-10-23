#include "ColliderBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
MyLib::ColliderBase::ColliderBase(Kind kind, bool isTrigger)
{
	this->kind = kind;
	this->isTrigger = isTrigger;
}
