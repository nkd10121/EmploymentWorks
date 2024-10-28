#pragma once
#include "DxLib.h"
#include <string>
#include <list>
#include <memory>

struct Sound;

/// <summary>
/// サウンド関係を管理するクラス
/// </summary>
class SoundManager
{
private:
	struct Sound
	{
		std::string id;
		std::string path;
		int handle;
		bool isEteral;
	};
private:
	// シングルトンパターンなのでコンストラクタはprivateに置く
	SoundManager() {};
	virtual ~SoundManager();

	static SoundManager* m_instance;	//インスタンス
public:

	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&) = delete;
	SoundManager& operator= (const SoundManager&&) = delete;

	/// <summary>
	/// SoundManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new SoundManager;
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
	/// サウンドを読み込む
	/// </summary>
	/// <param name="id">サウンドID</param>
	/// <param name="path">サウンドパス</param>
	/// <param name="isBGM">true = BGM,false = SE</param>
	void Load(std::string id, std::string path, bool isBGM,bool isEternal);

	/// <summary>
	/// すべてのリソースの読み込みが終了しているかどうかを取得
	/// </summary>
	/// <returns></returns>
	const bool IsLoaded()const;

	/// <summary>
	/// 常駐フラグがfalseのハンドルを全削除する
	/// </summary>
	void Clear();

	/// <summary>
	/// 指定した名前のサウンドを流す
	/// </summary>
	/// <param name="id">流したいサウンドネーム</param>
	/// <param name="isFromStart">true = 最初から,false = 途中から</param>
	void PlayBGM(std::string id, bool isFromStart);

	void PlaySE(std::string id);

	void FadeOutBGM(std::string id, int fadeFrame);

	/// <summary>
	/// 指定した名前のサウンドを止める
	/// </summary>
	/// <param name="id">止めたいサウンドネーム</param>
	void StopBGM(std::string id);

	/// <summary>
	/// 指定したサウンドが流れているどうか
	/// </summary>
	/// <param name="id">サウンドネーム</param>
	/// <returns>true = 流れている,false = 流れていない</returns>
	bool isPlayingSound(std::string id);

	void ChangeBGMVolume(float volume);
	void ChangeSEVolume(float volume);

private:
	/// <summary>
	/// 指定したハンドルのサウンドが流れているかどうか
	/// </summary>
	/// <param name="handle">チェックしたいハンドル</param>
	/// <returns>true = 流れている,false = 流れていない</returns>
	bool CheckPlaying(int handle) { return CheckSoundMem(handle); }

private:
	std::list<std::shared_ptr<Sound>> m_BGM;
	std::list<std::shared_ptr<Sound>> m_SE;

	float m_BGMvolume;	//bgmの音量　min:0.0f,max:1.0f
	float m_SEvolume;	//seの音量　min:0.0f,max:1.0f
};

