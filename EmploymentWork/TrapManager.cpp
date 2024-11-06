#include "TrapManager.h"

TrapManager* TrapManager::m_instance = nullptr;

TrapManager::~TrapManager()
{
	Clear();
}

const bool TrapManager::CheckNeighbor(std::list<Trap*> check) const
{
	for (auto& t : check)
	{
		if (t->isPlaced)	return false;
	}
	return true;
}

void TrapManager::AddTrapPos(Vec3 pos)
{
	Trap add;
	add.isPlaced = false;
	add.pos = pos;

	m_traps.emplace_back(add);
}

void TrapManager::Update()
{
}

void TrapManager::Draw()
{
#ifdef _DEBUG
	for(auto& pos : m_traps)
	{
		if (pos.isPlaced)
		{
			DrawSphere3D(pos.pos.ToVECTOR(), 2, 4, 0xffffff, 0xffffff, false);
		}
		else
		{
			DrawSphere3D(pos.pos.ToVECTOR(), 2, 4, 0xff0000, 0xff0000, false);
		}
		continue;
	}

	DrawSphere3D(debugTrap.pos.ToVECTOR(), 4, 4, 0x00ff00, 0x00ff00, false);

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
			if (abs((trap.pos - temp.pos).Length()) > 0.0f && abs((trap.pos - temp.pos).Length()) <= 12.0f)
			{
				trap.m_neighborTraps.emplace_back(&temp);
			}
		}
	}
}

void TrapManager::Clear()
{
	for (auto& trap : m_traps)
	{
		trap.m_neighborTraps.clear();
	}
	m_traps.clear();
}

void TrapManager::EstablishTrap(Vec3 playerPos, Vec3 targetPos,int slot)
{

	//線分の始点と終点を設定
	auto start = playerPos;
	auto end = playerPos + targetPos * 30;

	Trap* establish = nullptr;

	float defaultLength = 100.0f;

	//トラップ枠分回す
	for (auto& trap : m_traps)
	{
		bool isEst = true;

		//トラップが置かれていない
		if (!trap.isPlaced && trap.m_neighborTraps.size() == 8 && CheckNeighbor(trap.m_neighborTraps))
		{
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap.pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;
				establish = &trap;
			}
		}
	}

	establish->isPlaced = true;
	for (auto& trap : establish->m_neighborTraps)
	{
		trap->isPlaced = true;
	}
}

void TrapManager::GetPlayerInfo(Vec3 playerPos, Vec3 targetPos)
{
	//線分の始点と終点を設定
	auto start = playerPos;
	auto end = playerPos + targetPos * 60;

	float defaultLength = 100.0f;

	//設置可能なトラップの座標分回す
	for (auto trap : m_traps)
	{
		//トラップが置かれていない
		if (!trap.isPlaced && trap.m_neighborTraps.size() == 8 && CheckNeighbor(trap.m_neighborTraps))
		{
			//線分とトラップ設置可能座標の距離を計算する
			//float length = Segment_Point_MinLength(start.ConvertToVECTOR(), end.ConvertToVECTOR(), pos.ConvertToVECTOR());
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap.pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;
				debugTrap = trap;
			}
		}
	}
}
