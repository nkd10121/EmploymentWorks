#pragma once
#include <string>
#include <list>

/// <summary>
/// 3Dモデルのハンドルを管理するクラス
/// </summary>
class ModelManager
{
public:
	//モデル構造体
	struct Model
	{
		std::string path;		//モデルパス
		std::string modelId;	//モデルID
		int handle;				//モデルハンドル
		bool isEternal;			//常駐フラグ
	};

private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelManager() {};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ModelManager();

	static ModelManager* m_instance;	//インスタンス

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager(ModelManager&&) = delete;
	ModelManager& operator= (const ModelManager&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new ModelManager;
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

public:
	/// <summary>
	/// 指定したパスをモデルをロードする
	/// </summary>
	/// <param name="id">モデルID</param>
	/// <param name="path">モデルパス</param>
	/// <param name="isEternal">常駐フラグ</param>
	void Load(std::string id, std::string path, bool isEternal = false);

	/// <summary>
	/// モデルハンドルを取得する
	/// </summary>
	/// <param name="id">モデルID</param>
	/// <returns>-1 : エラー, -1以外 : モデルの複製ハンドル</returns>
	int GetModelHandle(std::string id);

	/// <summary>
	/// 常駐フラグがfalseのハンドルを全削除する
	/// </summary>
	void Clear();

	/// <summary>
	/// ハンドルが読み込まれているかどうか確認
	/// </summary>
	/// <returns>false : 読み込み終えていない, true : 読み込み終了</returns>
	bool IsLoaded();

private:
	std::list<Model> m_handles;	//モデルハンドル

};

