#include "ResourceManager.h"
#include "DxLib.h"
#include <cassert>
ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Load(std::vector<std::string> data)
{
	//すでにロードされていたら何もしない
	for (auto& resource : m_resources)
	{
		if (resource->id == id)
		{
#ifdef _DEBUG	//デバッグ描画
			assert(0 && "%sはすでにロードされています",id.c_str());
#endif
			return;
		}
	}

	//ここに来たということはすでにロードされていなかった
	//→新しくロードする必要がある
	Resource add;
	add.kind = CheckKind(extension);
	add.id = id;
	add.isEternal = isEternal;
	if (extension == ".mp3")
	{
	}


	m_handles.emplace_back(add);

#ifdef _DEBUG
	//ロードに失敗していたらエラーを吐くようにする
	assert(add.handle != -1 && "モデルのロードに失敗しました");
#endif
	return;
}

const ResourceManager::Kind ResourceManager::CheckKind(std::string ex) const
{
	Kind ret;

	//画像
	if (ex == ".png")
	{
		ret = Kind::Image;
		return;
	}
	//音
	else if (ex == ".mp3")
	{
		ret = Kind::Sound;
		return;
	}
	//モデル
	else if (ex == ".mv1")
	{
		ret = Kind::Model;
		return;
	}
	//エフェクト
	else if (ex == ".efk")
	{
		ret = Kind::Effect;
		return;
	}
	//シェーダー
	else if (ex == ".pso" || ex == ".vso")
	{
		ret = Kind::Shader;
		return;
	}

#ifdef _DEBUG
	assert(0 && "想定されていない拡張子です");
#endif
	return ret;
}
