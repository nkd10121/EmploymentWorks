#include "ColliderBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MyLib::ColliderBase::ColliderBase(Kind kind, bool isTrigger)
{
	this->kind = kind;
	this->isTrigger = isTrigger;
}
