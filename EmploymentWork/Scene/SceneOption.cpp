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
	//フェードアウトをスキップする
	//SkipFadeOut();
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
	DrawFormatString(0, 32, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneOption::SelectNextSceneUpdate()
{
	//Bボタンを押したら元のシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		EndThisScene();
		return;
	}
}
