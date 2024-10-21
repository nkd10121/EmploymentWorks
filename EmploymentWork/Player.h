#pragma once
#include "Vec3.h"

class Player
{
public:
	Player();
	virtual ~Player();

	void Init();
	void Update();
	void Draw();

private:
	MyLib::Vec3 m_pos;
	MyLib::Vec3 temp_moveVec;
};

