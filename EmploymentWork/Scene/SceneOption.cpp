#include "SceneOption.h"

namespace
{
}

SceneOption::SceneOption():
	SceneBase("SCENE_OPTION")
{
}

SceneOption::~SceneOption()
{
}

void SceneOption::StartLoad()
{
}

bool SceneOption::IsLoaded() const
{
	return true;
}

void SceneOption::Init()
{
}

void SceneOption::End()
{
}

void SceneOption::Update()
{
}

void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 16, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneOption::SelectNextSceneUpdate()
{
}
