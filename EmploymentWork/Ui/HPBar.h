#pragma once
#include <vector>
#include "Vec2.h"

class HPBar
{
public:
	HPBar();
	virtual ~HPBar();

	void Init(int max);
	void Update(int hp);
	void Draw();

private:
	int m_maxHP;
	Vec2 m_drawPos;

	int m_hpBarWidth;
	int m_hpBarHeight;

	int m_gaugeWidth;

	std::vector<int> m_handles;
};

