#include "HPBar.h"

#include "ImageManager.h"

namespace
{
	const std::string kId[] =
	{
		"I_HPBG",
		"I_HPGAUGEBG"
		"I_HPGAUGEDECREASE",
		"I_HPGAUGE",
	};

	constexpr float kHpBarScale = 0.225f;

}

HPBar::HPBar():
	m_maxHP(0),
	m_drawPos(),
	m_hpBarWidth(0),
	m_hpBarHeight(0),
	m_gaugeWidth(0),
	m_handles()
{

}

HPBar::~HPBar()
{
	for (auto& handle : m_handles)
	{
		DeleteGraph(handle);
	}
	m_handles.clear();
}

void HPBar::Init(int max)
{
	m_maxHP = max;

	for (auto& id : kId)
	{
		m_handles.push_back(ImageManager::GetInstance().GetHandle(id));
	}

	//HPバーの画像サイズを取得する
	GetGraphSize(m_handles[3], &m_hpBarWidth, &m_hpBarHeight);
}

void HPBar::Update(int hp)
{
	//HPバーの計算
	//プレイヤーのHP÷プレイヤーの最大HPで現在のHPの割合を出す
	float widthScale = static_cast<float>(hp) / static_cast<float>(m_maxHP);
	//HPの割合分の画像のX幅を出す
	m_gaugeWidth = static_cast<int>(m_hpBarWidth * widthScale);
	//何もしないと画像が中心によっていくため画像の中心座標をHPに応じてずらす
	m_drawPos.x = 240 - static_cast<int>((m_hpBarWidth - m_gaugeWidth) * kHpBarScale / 2);
	m_drawPos.y = 30;

}

void HPBar::Draw()
{
	//Hpバーの描画
	DrawRotaGraph(220, 30, kHpBarScale, 0.0f, m_handles[0], true);
	DrawRotaGraph(236, 30, kHpBarScale, 0.0f, m_handles[2], true);
	DrawRectRotaGraph(m_drawPos.x, m_drawPos.y, 0, 0, m_gaugeWidth, m_hpBarHeight, kHpBarScale, 0.0f, m_handles[1], true);
	DrawRotaGraph(236, 25, kHpBarScale, 0.0f, m_handles[3], true);
}
