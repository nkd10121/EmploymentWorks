#pragma once
#include "Vec3.h"
#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLib::Physics> physics);
	void Update();
	void Draw();

	// è’ìÀÇµÇΩÇ∆Ç´
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

private:
	MyLib::Vec3 m_pos;
	MyLib::Vec3 temp_moveVec;
};

