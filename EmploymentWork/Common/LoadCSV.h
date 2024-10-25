#pragma once
#include "CharacterBase.h"

class LoadCSV
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	LoadCSV() {};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LoadCSV() {};

	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static LoadCSV* m_instance;
public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	LoadCSV(const LoadCSV&) = delete;
	LoadCSV& operator=(const LoadCSV&) = delete;
	LoadCSV(LoadCSV&&) = delete;
	LoadCSV& operator= (const LoadCSV&&) = delete;

	/// <summary>
	/// LoadCSVはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static LoadCSV& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCSV;
		}

		return *m_instance;
	}

	//これをし忘れると普通にメモリリーク
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	/// <summary>
	/// ステータス情報を取得
	/// </summary>
	/// <param name="characterName">ステータスを取得したいキャラクター名</param>
	CharacterBase::Status LoadStatus(const char* characterName);

	/// <summary>
	/// そのシーンでロードするリソース名を取得
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	std::map<std::string, std::string> GetLoadResourcePath(std::string stageId);
};