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
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneRanking::SelectNextSceneUpdate()
{
}
