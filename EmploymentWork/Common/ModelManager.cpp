#include "ModelManager.h"
#include "DxLib.h"
#include <cassert>

ModelManager* ModelManager::m_instance = nullptr;

namespace
{
}

/// <summary>
/// デストラクタ
/// </summary>
ModelManager::~ModelManager()
{
	//持っているハンドルを全部Deleteする
	for (auto& h : m_handles)
	{
		MV1DeleteModel(h.handle);
	}

	m_handles.clear();
}

/// <summary>
/// 指定したパスをモデルをロードする
/// </summary>
void ModelManager::LoadModel(std::string path, bool isEternal)
{
	//すでにロードされていたら何もしない
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
#ifdef _DEBUG
			//指定したパスのモデルがすでにロードされていたらエラーを吐くようにする
			assert(0 && "そのモデルはすでにロードされています");
#endif
			return;
		}
	}

	//ここに来たということはすでにロードされていなかった
	//→新しくロードする必要がある
	Model add;
	add.handle = MV1LoadModel(path.c_str());
	add.path = path;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

#ifdef _DEBUG
	//モデルのロードに失敗していたらエラーを吐くようにする
	assert(add.handle != -1 && "モデルのロードに失敗しました");
#endif
	return;
}

/// <summary>
/// モデルハンドルを取得する
/// </summary>
int ModelManager::GetModelHandle(std::string path)
{
	//ロードされていたら複製ハンドルを返す
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
			return MV1DuplicateModel(h.handle);
		}
	}

	//ここまで来たということはロードされていなかった
#ifdef _DEBUG
	//念のためassertを仕込んでおく
	assert(0 && "指定したパスはロードされていません");
#endif
	return -1;
}

/// <summary>
/// 常駐フラグがfalseのハンドルを削除する
/// </summary>
void ModelManager::Clear()
{
	//isEternalがfalseのハンドルをDeleteする
	for (auto& h : m_handles)
	{
		if (!h.isEternal)
		{
			MV1DeleteModel(h.handle);
		}
	}

	//不要になったハンドルをここで削除処理する
	auto it = remove_if(m_handles.begin(), m_handles.end(), [](auto& v) {
		return v.isEternal == false;
		});
	m_handles.erase(it, m_handles.end());
}

/// <summary>
/// ハンドルが読み込まれているかどうか確認
/// </summary>
bool ModelManager::IsLoaded()
{
	for (auto& h : m_handles)
	{
		if (CheckHandleASyncLoad(h.handle))	return false;
	}

	return true;
}