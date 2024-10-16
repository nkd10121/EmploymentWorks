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
};

