#include "ShaderManager.h"
#include "DxLib.h"
#include <cassert>

ShaderManager* ShaderManager::m_instance = nullptr;

ShaderManager::~ShaderManager()
{
	m_handles.clear();
}

void ShaderManager::Load(std::string id, std::string path, bool isPixel, bool isEternal)
{
	//すでにロードされていたら何もしない
	for (auto& h : m_handles)
	{
		if (h.id == id)
		{
#ifdef _DEBUG	//デバッグ描画
			//指定したパスのモデルがすでにロードされていたらエラーを吐くようにする
			assert(0 && "そのモデルはすでにロードされています");
#endif
			return;
		}
	}

	//ここに来たということはすでにロードされていなかった
	//→新しくロードする必要がある
	Shader add;

	if (isPixel)
	{
		add.handle = LoadPixelShader(path.c_str());
	}
	else
	{
		add.handle = LoadVertexShader(path.c_str());
	}
	add.id = id;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

#ifdef _DEBUG
	//モデルのロードに失敗していたらエラーを吐くようにする
	assert(add.handle != -1 && "モデルのロードに失敗しました");
#endif
	return;
}

int ShaderManager::GetHandle(std::string id)
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
	assert(0 && "指定したモデルIDはロードされていません");
#endif
	return -1;
}

void ShaderManager::Clear()
{
	//不要になったハンドルをここで削除処理する
	auto it = remove_if(m_handles.begin(), m_handles.end(), [](auto& v) {
		return v.isEternal == false;
		});
	m_handles.erase(it, m_handles.end());
}

bool ShaderManager::IsLoaded()
{
	for (auto& h : m_handles)
	{
		if (CheckHandleASyncLoad(h.handle))	return false;
	}

	return true;
}
