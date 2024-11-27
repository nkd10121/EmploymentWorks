#include "ImageManager.h"
#include "DxLib.h"
#include <cassert>

ImageManager* ImageManager::m_instance = nullptr;

ImageManager::~ImageManager()
{
	//持っているハンドルを全部Deleteする
	for (auto& h : m_handles)
	{
		DeleteGraph(h.handle);
	}

	m_handles.clear();
}

void ImageManager::Load(std::string id, std::string path, bool isEternal)
{
	//すでにロードされていたら何もしない
	for (auto& h : m_handles)
	{
		if (h.id == id)
		{
#ifdef _DEBUG	//デバッグ描画
			//指定したパスのモデルがすでにロードされていたらエラーを吐くようにする
			assert(0 && "その画像はすでにロードされています");
#endif
			return;
		}
	}

	//ここに来たということはすでにロードされていなかった
	//→新しくロードする必要がある
	Image add;
	add.handle = LoadGraph(path.c_str());
	add.id = id;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

#ifdef _DEBUG
	//モデルのロードに失敗していたらエラーを吐くようにする
	assert(add.handle != -1 && "モデルのロードに失敗しました");
#endif
	return;
}

int ImageManager::GetHandle(std::string id)
{
	//ロードされていたら複製ハンドルを返す
	for (auto& h : m_handles)
	{
		if (h.id == id)
		{
			return h.handle;
		}
	}

	//ここまで来たということはロードされていなかった
#ifdef _DEBUG
	//念のためassertを仕込んでおく
	assert(0 && "指定したIDはロードされていません");
#endif
	return -1;
}

void ImageManager::Clear()
{
	//isEternalがfalseのハンドルをDeleteする
	for (auto& h : m_handles)
	{
		if (!h.isEternal)
		{
			DeleteGraph(h.handle);
		}
	}

	//不要になったハンドルをここで削除処理する
	auto it = remove_if(m_handles.begin(), m_handles.end(), [](auto& v) {
		return v.isEternal == false;
		});
	m_handles.erase(it, m_handles.end());
}

bool ImageManager::IsLoaded()
{
	for (auto& h : m_handles)
	{
		if (CheckHandleASyncLoad(h.handle))	return false;
	}

	return true;
}
