#pragma once
#include "Vec3.h"
#include <memory>
#include <list>

//#include "TrapBase.h"

class TrapManager
{
private:
	struct Trap
	{
		//std::shared_ptr<TrapBase> trap;
		Vec3 pos;
		bool isPlaced;
		std::list<Trap*> m_neighborTraps;
	};
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TrapManager() {};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TrapManager();

	static TrapManager* m_instance;	//インスタンス

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	TrapManager(const TrapManager&) = delete;
	TrapManager& operator=(const TrapManager&) = delete;
	TrapManager(TrapManager&&) = delete;
	TrapManager& operator= (const TrapManager&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static TrapManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new TrapManager;
		}

		return *m_instance;
	}

	/// <summary>
	/// 削除
	/// これをし忘れると普通にメモリリーク
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

private:
	const bool CheckNeighbor(std::list<Trap*> check)const;
public:
	void AddTrapPos(Vec3 pos);

	void Update();

	void Draw();

	void SetUp();

	void Clear();

	void EstablishTrap(Vec3 playerPos,Vec3 targetPos,int slot);


	//デバッグ用
	void GetPlayerInfo(Vec3 playerPos, Vec3 targetPos);
private:
	std::list<Trap> m_traps;


	//デバッグ用
	Trap debugTrap;
};

