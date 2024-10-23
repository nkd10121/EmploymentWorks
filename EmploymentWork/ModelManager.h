#pragma once
#include <string>
#include <list>



/// <summary>
/// 3Dモデルのハンドルを管理するクラス
/// </summary>
class ModelManager
{
public:
	/// <summary>
	/// ハンドル
	/// </summary>
	struct Handle
	{
		std::string path;	//モデルパス
		int handle;			//モデルハンドル
		bool isEternal;		//常駐フラグ
	};

private:
	//コンストラクタ
	ModelManager() {};
	//デストラクタ
	virtual ~ModelManager();
	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static ModelManager* m_instance;

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager(ModelManager&&) = delete;
	ModelManager& operator= (const ModelManager&&) = delete;

	/// <summary>
	/// ModelManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static ModelManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new ModelManager;
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
	/// 指定したパスをモデルをロードする
	/// </summary>
	/// <param name="path">モデルパス</param>
	/// <param name="isEternal">常駐フラグ</param>
	void LoadModel(std::string path, bool isEternal = false);

	/// <summary>
	/// モデルハンドルを取得する
	/// </summary>
	/// <param name="path">モデルパス</param>
	/// <returns>-1 : エラー, -1以外 : モデルの複製ハンドル</returns>
	int GetModelHandle(std::string path);

	/// <summary>
	/// 常駐フラグがfalseのハンドルを全削除する
	/// </summary>
	void Clear();

	/// <summary>
	/// ロードされているすべてのモデルの複製前ハンドルを返す
	/// </summary>
	/// <returns></returns>
	const std::list<int> GetPrimitiveModelHandles()const;

private:
	//モデルハンドル
	std::list<Handle> m_handles;

};

