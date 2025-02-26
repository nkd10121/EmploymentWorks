﻿#include "SoundManager.h"
#include <cassert>

#include "Setting.h"
#include "ResourceManager.h"

SoundManager* SoundManager::m_instance = nullptr;


namespace
{
	constexpr float kFadeFrame = 30.0f;
}

/// <summary>
/// デストラクタ
/// </summary>
SoundManager::~SoundManager()
{
	//念のため全消去しておく
	Clear();
}

/// <summary>
/// サウンドを読み込む
/// </summary>
void SoundManager::Load(std::string id, std::string path, bool isBGM, bool isEternal)
{
	//BGMの場合
	if (isBGM)
	{
		//		//すでにロードされているかチェックする
		//		for (auto& bgm : m_BGM)
		//		{
		//			//同じパスのデータがすでにロードされていたら
		//			if (bgm.path == path)
		//			{
		//				//何もしない
		//#ifdef _DEBUG	//デバッグ描画 
		//				//念のためエラーを吐くようにする
		//				assert(0 && "そのサウンドはすでにロードされています");
		//#endif
		//				return;
		//			}
		//		}
		//
		//		//ここまできたらロードする
		//		Sound add;
		//		add.id = id;
		//		add.path = path;
		//		add.handle = LoadSoundMem(path.c_str());
		//		add.isEteral = isEternal;
		//		m_BGM.emplace_back(add);
		//
		//		return;
	}
	//SEの場合
	else
	{
		//すでにロードされているかチェックする
		for (auto& se : m_SE)
		{
			//同じパスのデータがすでにロードされていたら
			if (se->path == path)
			{
				//何もしない
#ifdef _DEBUG	//デバッグ描画 
				//指定したパスのサウンドデータがすでにロードされていたらエラーを吐くようにする
				assert(0 && "そのサウンドはすでにロードされています");
#endif
				return;
			}
		}

		//ここまできたらロードする
		std::shared_ptr<Sound> add = std::make_shared<Sound>();
		add->id = id;
		add->path = path;
		add->handle = LoadSoundMem(path.c_str());
		add->isEteral = isEternal;
		m_SE.emplace_back(add);

		return;
	}
}

/// <summary>
/// すべてのリソースの読み込みが終了しているかどうかを取得
/// </summary>
const bool SoundManager::IsLoaded() const
{
	//for (auto& bgm : m_BGM)
	//{
	//	if (CheckHandleASyncLoad(bgm.handle)) return false;
	//}

	//for (auto& se : m_SE)
	//{
	//	if (CheckHandleASyncLoad(se->handle)) return false;
	//}

	return true;
}

/// <summary>
/// 常駐フラグがfalseのハンドルを全削除する
/// </summary>
void SoundManager::Clear()
{
	m_BGM = {};

	for (auto& se : m_SE)
	{
		DeleteSoundMem(se->handle);
	}
	m_SE.clear();

	//念のため全消去しておく
	InitSoundMem();
}

/// <summary>
/// 指定したIDのBGMを流す
/// </summary>
void SoundManager::PlayBGM(std::string id, bool isFromStart)
{
	//for (auto& bgm : m_BGM)
	//{
	//	//指定したIDと一致するハンドルが存在していたら
	//	if (bgm->id == id)
	//	{
	//		//流れていたら何もしない
	//		if (CheckPlaying(bgm->handle))
	//		{
	//			return;
	//		}

	//		//流れていなかったら流す
	//		PlaySoundMem(bgm->handle, DX_PLAYTYPE_BACK, isFromStart);
	//		return;
	//	}
	//}

	auto playHandle = ResourceManager::GetInstance().GetHandle(id);
	ChangeVolumeSoundMem(static_cast<int>(255 * Setting::GetInstance().GetMasterVolume() * Setting::GetInstance().GetBGMVolume()), playHandle);
	//流れていたら何もしない
	if (!CheckPlaying(playHandle))
	{
		//流れていなかったら流す
		PlaySoundMem(playHandle, DX_PLAYTYPE_BACK, isFromStart);

		Sound add;
		add.handle = playHandle;
		add.id = id;
		add.isEteral = false;
		add.path = "";

		m_BGM = add;

		return;
	}
	//
	//	//ここまできたらなにかしらの理由でサウンドを流すのを失敗している
	//#ifdef _DEBUG	//デバッグ描画
	//	assert(0 && "BGMを流すのに失敗しました。");
	//	//指定したIDがスペルミスしているか、ロードできていないかが原因
	//#endif
	return;
}

/// <summary>
///	指定したIDのSEを流す
/// </summary>
void SoundManager::PlaySE(std::string id)
{
	//for (auto& se : m_SE)
	//{
	//	//指定したIDと一致するハンドルが存在していたら
	//	if (se->id == id)
	//	{
	//		//流す
	//		PlaySoundMem(se->handle, DX_PLAYTYPE_BACK, true);
	//		return;
	//	}
	//}

	auto playHandle = ResourceManager::GetInstance().GetHandle(id);
	auto volume = static_cast<int>(255 * Setting::GetInstance().GetMasterVolume() * Setting::GetInstance().GetSEVolume());
	ChangeVolumeSoundMem(volume, playHandle);
	PlaySoundMem(playHandle, DX_PLAYTYPE_BACK, true);

	return;

	//ここまできたらなにかしらの理由でサウンドを流すのを失敗している
#ifdef _DEBUG	//デバッグ描画
	assert(0 && "SEを流すのに失敗しました。");
	//指定したIDがスペルミスしているか、ロードできていないかが原因
#endif
	return;
}

/// <summary>
/// BGMをフェードアウトさせる
/// </summary>
void SoundManager::FadeOutBGM(std::string id, int fadeFrame)
{

	//指定したIDと一致するハンドルが存在していたら
	if (m_BGM.id == id)
	{
		//BGMvolume = dif;
		auto volume = GetVolumeSoundMem2(m_BGM.handle);

		if (volume == 0)
		{
			m_fadeFrameCount = 0;
			return;
		}
	}


	auto volume = static_cast<float>(255 * Setting::GetInstance().GetMasterVolume() * Setting::GetInstance().GetBGMVolume());
	float dif = volume * ((fadeFrame - static_cast<float>(m_fadeFrameCount)) / fadeFrame);

	//指定したIDと一致するハンドルが存在していたら
	if (m_BGM.id == id)
	{
		//BGMvolume = dif;
		ChangeVolumeSoundMem(static_cast<int>(dif), m_BGM.handle);

		if (255 * dif <= 0.0f)
		{
			StopSoundMem(m_BGM.handle);
		}

		m_fadeFrameCount++;
		return;
	}

	return;
}

/// <summary>
/// 指定したIDのBGMを止める
/// </summary>
void SoundManager::StopBGM(std::string id)
{
	//流れていなかったら何もしない
	if (!CheckPlaying(m_BGM.handle))
	{
		return;
	}

	StopSoundMem(m_BGM.handle);

	return;
}

/// <summary>
/// 指定したサウンドが流れているどうか
/// </summary>
bool SoundManager::isPlayingSound(std::string id)
{
	//指定したIDと一致するハンドルが存在していたら
	if (m_BGM.id == id)
	{
		//流れているかどうかを返す
		if (CheckPlaying(m_BGM.handle))
		{
			return true;
		}
		else
		{
			return false;
		}

	}
#ifdef _DEBUG	//デバッグ描画
	assert(0 && "指定したIDは存在しません。");
#endif
	return false;
}


void SoundManager::BGMChangeVolume()
{
	//もし流れていたら
	if (CheckPlaying(m_BGM.handle))
	{
		StopSoundMem(m_BGM.handle);
	}

	ChangeVolumeSoundMem(static_cast<int>(255 * Setting::GetInstance().GetMasterVolume() * Setting::GetInstance().GetBGMVolume()), m_BGM.handle);
	PlaySoundMem(m_BGM.handle, DX_PLAYTYPE_BACK, false);
	return;
}
