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
	Vec2 m_drawRedGaugePos;
	Vec2 m_drawYellowGaugePos;
	Vec2 m_drawPos;

	Vec2 m_offset;

	int m_hpBarWidth;
	int m_hpBarHeight;

	int m_gaugeWidth;
	int m_gaugeYellowWidth;

	int m_vibrationCount;

	std::vector<int> m_handles;
};

