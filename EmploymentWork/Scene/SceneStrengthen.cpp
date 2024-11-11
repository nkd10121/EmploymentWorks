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
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneStrengthen::SelectNextSceneUpdate()
{
}
