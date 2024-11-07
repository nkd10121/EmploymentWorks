#include "TrapManager.h"
#include "MathHelp.h"

TrapManager* TrapManager::m_instance = nullptr;

TrapManager::TrapManager()
{
}

TrapManager::~TrapManager()
{
	Clear();
}

const bool TrapManager::CheckNeighbor(std::list<std::weak_ptr<Trap>> check) const
{
	for (auto& t : check)
	{
		if (t.lock()->isPlaced)	return false;
	}
	return true;
}

void TrapManager::AddTrapPos(Vec3 pos)
{
	std::shared_ptr<Trap> add = std::make_shared<Trap>();
	add->isPlaced = false;
	add->pos = pos;
	add->neighborTraps.clear();
	m_traps.emplace_back(add);
}

void TrapManager::Update()
{
}

void TrapManager::Draw()
{
#ifdef _DEBUG
	//for (auto& pos : m_traps)
	//{
	//	if (pos->isPlaced)
	//	{
	//		DrawSphere3D(pos->pos.ToVECTOR(), 3, 4, 0xffffff, 0xffffff, false);
	//	}
	//	else
	//	{
	//		DrawSphere3D(pos->pos.ToVECTOR(), 3, 4, 0x000000, 0x000000, false);
	//	}
	//}

	if (debugTrap != nullptr)
	{
		DrawSphere3D(debugTrap->pos.ToVECTOR(), 4, 4, 0x00ff00, 0x00ff00, false);
		DrawFormatString(0, 220, 0xffffff, "%d", debugTrap->neighborTraps.size());
	}

	//for (auto& temp : m_traps.front().m_neighborTraps)
	//{
	//	DrawSphere3D(temp.pos.ToVECTOR(), 2, 12, 0x0000ff, 0x0000ff, false);
	//}

#endif
}

void TrapManager::SetUp()
{
	for (auto& trap : m_traps)
	{
		for (auto& temp : m_traps)
		{
			if (abs((trap->pos - temp->pos).Length()) > 0.0f && abs((trap->pos - temp->pos).Length()) <= 12.0f)
			{
				trap->neighborTraps.emplace_back(temp);
			}
		}
	}
}

void TrapManager::Clear()
{
	for (auto& trap : m_traps)
	{
		trap->neighborTraps.clear();
	}
	m_traps.clear();
}

void TrapManager::EstablishTrap(Vec3 playerPos, Vec3 targetPos, int slot)
{
	auto& sa = debugTrap;
	sa->isPlaced = true;


	for (auto& trap : debugTrap->neighborTraps)
	{
		trap.lock()->isPlaced = true;
	}
}

void TrapManager::SelectPoint(Vec3 playerPos, Vec3 targetPos)
{
#ifdef TRUE	//バウンディングボックスを使った処理方法
	//線分の始点と終点を設定
	auto start = playerPos;
	auto end = playerPos + targetPos * 60;

	float defaultLength = 100.0f;

	DrawCube3D(start.ToVECTOR(), end.ToVECTOR(), 0xff0000, 0xff0000, false);

	//startとendを始点終点とした四角形との当たり判定をまずとる

	auto hit = CheckHitBoundingBoxAndPoints(start, end, m_traps);

	//設置可能なトラップの座標分回す
	for (auto& trap : hit)
	{
		//トラップが置かれていない
		if (!trap->isPlaced && trap->neighborTraps.size() == 8 && CheckNeighbor(trap->neighborTraps))
		{
			//線分とトラップ設置可能座標の距離を計算する
			//float length = Segment_Point_MinLength(start.ConvertToVECTOR(), end.ConvertToVECTOR(), pos.ConvertToVECTOR());
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap->pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;

				debugTrap = trap;
				//debugTrap->isPlaced = trap.isPlaced;
				//debugTrap->neighborTraps = trap.neighborTraps;
				//debugTrap->pos = trap.pos;
			}
		}
	}
#else		//バウンディングボックスを使わずにごり押し処理方法
	//線分の始点と終点を設定
	auto start = playerPos;
	auto end = playerPos + targetPos * 60;

	float defaultLength = 100.0f;

	//設置可能なトラップの座標分回す
	for (auto& trap : m_traps)
	{
		//トラップが置かれていない
		if (!trap->isPlaced && trap->neighborTraps.size() == 8 && CheckNeighbor(trap->neighborTraps))
		{
			//線分とトラップ設置可能座標の距離を計算する
			//float length = Segment_Point_MinLength(start.ConvertToVECTOR(), end.ConvertToVECTOR(), pos.ConvertToVECTOR());
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap->pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;

				debugTrap = trap;
				//debugTrap->isPlaced = trap.isPlaced;
				//debugTrap->neighborTraps = trap.neighborTraps;
				//debugTrap->pos = trap.pos;
			}
		}
	}
#endif
}
