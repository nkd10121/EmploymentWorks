#include "SceneStageSelect.h"

#include "SceneGame.h"
#include "SceneSelect.h"

SceneStageSelect::SceneStageSelect():
	SceneBase("SCENE_STAGESELECT")
{
}

SceneStageSelect::~SceneStageSelect()
{
}

void SceneStageSelect::StartLoad()
{
}

bool SceneStageSelect::IsLoaded() const
{
	return true;
}

void SceneStageSelect::Init()
{
}

void SceneStageSelect::End()
{
}

void SceneStageSelect::Update()
{
}

void SceneStageSelect::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneStageSelect::SelectNextSceneUpdate()
{
	//決定ボタンを押したら現在選択しているシーンに遷移する
	if (Input::GetInstance().IsTriggered("OK"))
	{
		//ゲームシーンに遷移する
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}

	//Bボタンを押したらセレクトシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneSelect>());
		EndThisScene();
		return;
	}
}