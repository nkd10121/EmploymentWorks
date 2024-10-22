#pragma once
#include "StateBase.h"

class PlayerStateIdle : public StateBase
{
public:
	PlayerStateIdle(std::shared_ptr<CharacterBase> own);

	void Init();

	virtual void Update() override;

	virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;
};