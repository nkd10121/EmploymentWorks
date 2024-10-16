#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"

SceneGame::SceneGame():
	temp_handle()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::StartLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	// TODO:この間でリソースをロードする
	for (int i = 0; i < 5; i++)
	{
		temp_handle.push_back(MV1LoadModel("data/model/player.mv1"));
	}

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneGame::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する
	for (auto& h : temp_handle)
	{
		if (CheckHandleASyncLoad(h))	return false;
	}

	return true;
}

void SceneGame::Init()
{
	//TODO:ここで実態の生成などをする
}

void SceneGame::End()
{
	//TODO:ここでリソースのメモリ開放などをする
	for (auto& h : temp_handle)
	{
		MV1DeleteModel(h);
	}
	temp_handle.clear();
}

void SceneGame::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}

	if (Input::GetInstance().IsTriggered("Y"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}
}

void SceneGame::Draw()
{
	for (auto& h : temp_handle)
	{
		MV1DrawModel(h);
	}

#ifdef _DEBUG
	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
