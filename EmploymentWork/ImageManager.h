#pragma once
#include <string>
#include <list>

/// <summary>
/// 画像を管理するクラス
/// </summary>
class ImageManager
{
public:
	//画像構造体
	struct Image
	{
		std::string id;	//ID
		int handle;				//ハンドル
		bool isEternal;			//常駐フラグ
	};
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ImageManager() {};
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ImageManager();

	static ImageManager* m_instance;	//インスタンス

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	ImageManager(const ImageManager&) = delete;
	ImageManager& operator=(const ImageManager&) = delete;
	ImageManager(ImageManager&&) = delete;
	ImageManager& operator= (const ImageManager&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static ImageManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new ImageManager;
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
	/// 指定したパスをロードする
	/// </summary>
	/// <param name="id">ID</param>
	/// <param name="path">パス</param>
	/// <param name="isEternal">常駐フラグ</param>
	void Load(std::string id, std::string path, bool isEternal = false);

	/// <summary>
	/// ハンドルを取得する
	/// </summary>
	/// <param name="id">モデルID</param>
	/// <returns>-1 : エラー, -1以外 : ハンドル</returns>
	int GetHandle(std::string id);

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
	std::list<Image> m_handles;		
};