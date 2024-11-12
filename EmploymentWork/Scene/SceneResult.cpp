#include "SceneResult.h"

SceneResult::SceneResult():
	SceneBase("SCENE_RESULT")
{
}

SceneResult::~SceneResult()
{
}

void SceneResult::StartLoad()
{
}

bool SceneResult::IsLoaded() const
{
	return true;
}

void SceneResult::Init()
{
}

void SceneResult::End()
{
}

void SceneResult::Update()
{
}

void SceneResult::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneResult::SelectNextSceneUpdate()
{
}
