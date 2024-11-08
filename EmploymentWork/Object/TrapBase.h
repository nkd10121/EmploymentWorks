#pragma once
#include "ObjectBase.h"
class TrapBase : public ObjectBase
{
public:
	TrapBase(GameObjectTag tag);
	~TrapBase();

	void Init();
	void Update();
	void Draw();

};

