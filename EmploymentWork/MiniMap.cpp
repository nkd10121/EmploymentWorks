#include "MiniMap.h"
#include "ResourceManager.h"
#include "DrawUI.h"

namespace
{
	// 右上のUI描画位置とスケール
	constexpr int kRightUiX = 1180;
	constexpr int kRightUiY1 = 150;
	constexpr int kRightUiY2 = 45;
	constexpr int kRightUiY3 = 40;
	constexpr float kRightUiScale1 = 0.9f;
	constexpr float kRightUiScale2 = 0.75f;

	constexpr int kMiniMapWidth = 180;
	constexpr int kMiniMapHeight = 180;
}

MiniMap::MiniMap()
{
	m_ironHandle = ResourceManager::GetInstance().GetHandle("I_IRONUI");
	m_bgHandle = ResourceManager::GetInstance().GetHandle("I_MINIMAPBG");
}

MiniMap::~MiniMap()
{
}

void MiniMap::Init(int mapHandle, Vec3 crystalPos)
{
	m_minimapHandle = mapHandle;

	auto mapOrigin = Vec3(0.0f, 0.0f, 72.0f);
	// スケーリングを適用
	float x = (crystalPos.x - mapOrigin.x) * 0.42f;
	float y = (crystalPos.z - mapOrigin.z) * 0.42f;

	// オフセットを適用
	m_crystalPosOnMinimap.x = x + kRightUiX;
	m_crystalPosOnMinimap.y = -y + kRightUiY1;
}

void MiniMap::Update(Vec3 playerPos, std::list<Vec3> enemyPoss)
{

	auto mapOrigin = Vec3(0.0f, 0.0f, 72.0f);

	m_playerPosOnMinimap = ConvertToMiniMapCoords(playerPos, mapOrigin, 0.42f, Vec2(kRightUiX, kRightUiY1));

	m_enemyPosOnMinimap.clear();
	for (auto& pos : enemyPoss)
	{
		m_enemyPosOnMinimap.emplace_back(ConvertToMiniMapCoords(pos, mapOrigin, 0.42f, Vec2(kRightUiX, kRightUiY1)));
	}
}

void MiniMap::Draw()
{
	// 操作説明UIの描画
	DrawUI::GetInstance().RegisterDrawRequest([=]()
	{
		// 右上のUI描画
		DrawRotaGraph(kRightUiX, kRightUiY1, kRightUiScale1, 0.0f, m_bgHandle, true);
		DrawRotaGraph(kRightUiX, kRightUiY2, kRightUiScale2, 0.0f, m_ironHandle, true);
		DrawRotaGraph(kRightUiX, kRightUiY1, 0.72f, 0.0f, m_minimapHandle, true);

		DrawCircle(m_crystalPosOnMinimap.x, m_crystalPosOnMinimap.y, 4, 0x0000ff, true);
		DrawCircle(m_playerPosOnMinimap.x, m_playerPosOnMinimap.y, 2, 0x00ff00, true);

		for (auto& enemyPos : m_enemyPosOnMinimap)
		{
			DrawCircle(enemyPos.x, enemyPos.y, 2, 0xff0000, true);
		}
	}, 0);

}

Vec2 MiniMap::ConvertToMiniMapCoords(const Vec3& worldPos, const Vec3& mapOrigin, float scale, const Vec2& miniMapCenter)
{
	Vec2 ret;

	// スケーリングを適用
	float x = (worldPos.x - mapOrigin.x) * scale;
	float y = (worldPos.z - mapOrigin.z) * scale;

	// オフセットを適用
	ret.x = x + miniMapCenter.x;
	ret.y = -y + miniMapCenter.y;

	return ret;
}