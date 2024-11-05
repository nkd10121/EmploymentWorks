#pragma once
class TrapManager
{
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
};

