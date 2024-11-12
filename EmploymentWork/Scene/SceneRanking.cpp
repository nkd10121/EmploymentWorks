#include "SceneRanking.h"

SceneRanking::SceneRanking():
	SceneBase("SCENE_RANKING")
{
}

SceneRanking::~SceneRanking()
{
}

void SceneRanking::StartLoad()
{
}

bool SceneRanking::IsLoaded() const
{
	return true;
}

void SceneRanking::Init()
{
	//フェードアウトをスキップする
	//SkipFadeOut();
}

void SceneRanking::End()
{
}

void SceneRanking::Update()
{
}

void SceneRanking::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 32, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneRanking::SelectNextSceneUpdate()
{
	//Bボタンを押したら元のシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		EndThisScene();
		return;
	}
}
