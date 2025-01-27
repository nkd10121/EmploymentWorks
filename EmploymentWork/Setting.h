#pragma once
class Setting
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Setting() {};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Setting();

	static Setting* m_instance;	//インスタンス

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	Setting(const Setting&) = delete;
	Setting& operator=(const Setting&) = delete;
	Setting(Setting&&) = delete;
	Setting& operator= (const Setting&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Setting& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Setting;
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

