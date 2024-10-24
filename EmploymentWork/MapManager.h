#pragma once
#include "Vec3.h"
#include <string>
#include <vector>
#include <unordered_map>

class MapManager
{
private:
	// シングルトンパターンなのでコンストラクタはprivateに置く
	MapManager();
	//コンストラクタ
	virtual ~MapManager();

	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static MapManager* m_instance;

	//配置情報データ
	struct LocationData
	{
		std::string name;	//名前
		std::string tag;	//タグ
		MyLib::Vec3 pos;	//座標
		MyLib::Vec3 rot;	//回転
		MyLib::Vec3 scale;	//大きさ
		int handle;
	};

	struct EnemyRoute
	{
		std::vector<MyLib::Vec3> start;
		std::vector<std::vector<MyLib::Vec3>> pos;
		MyLib::Vec3 end;
	};

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
	MapManager(MapManager&&) = delete;
	MapManager& operator= (const MapManager&&) = delete;

	/// <summary>
	/// MapManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static MapManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new MapManager;
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
	void Init();

	//ステージの生成に必要なモデルの読み込み
	void LoadModel();
	//ステージの生成に必要なモデルの削除
	void DeleteModel();

	//ステージ情報を読み込む
	void Load(const char* stageName);
	//ステージを描画する
	void Draw();

private:
	//マップ情報
	std::vector<LocationData> m_data;
	//モデルハンドル
	std::unordered_map<std::string, int> m_handles;
};

