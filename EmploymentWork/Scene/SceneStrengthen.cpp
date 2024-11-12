#include "SceneStrengthen.h"

SceneStrengthen::SceneStrengthen():
	SceneBase("SCENE_STRENGTHEN")
{
}

SceneStrengthen::~SceneStrengthen()
{
}

void SceneStrengthen::StartLoad()
{
}

bool SceneStrengthen::IsLoaded() const
{
	return true;
}

void SceneStrengthen::Init()
{
	//フェードアウトをスキップする
	//SkipFadeOut();
}

void SceneStrengthen::End()
{
}

void SceneStrengthen::Update()
{
}

void SceneStrengthen::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 32, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneStrengthen::SelectNextSceneUpdate()
{
	//Bボタンを押したら元のシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		SceneManager::GetInstance().PopScene();
		return;
	}
}
