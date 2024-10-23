#include "ModelManager.h"
#include "DxLib.h"
#include <cassert>

ModelManager* ModelManager::m_instance = nullptr;

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
	Handle add;
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
			h.handle = 0;
		}
	}

	//不要になったハンドルをここで削除処理する
	auto it = remove_if(m_handles.begin(), m_handles.end(), [](auto& v) {
		return v.handle == 0;
		});
	m_handles.erase(it, m_handles.end());
}

const std::list<int> ModelManager::GetPrimitiveModelHandles() const
{
	std::list<int> ret;

	for (auto& h : m_handles)
	{
		ret.emplace_back(h.handle);
	}

	return ret;
}
