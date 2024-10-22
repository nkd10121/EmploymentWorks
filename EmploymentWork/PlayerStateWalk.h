#pragma once
#include "StateBase.h"

class PlayerStateWalk : public StateBase
{
public:
	PlayerStateWalk(std::shared_ptr<CharacterBase> own);

	void Init();

	virtual void Update() override;

	virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;
};