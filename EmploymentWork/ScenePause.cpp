#include "ScenePause.h"
#include "Game.h"
#include "SceneTitle.h"

ScenePause::ScenePause()
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::StartLoad()
{
	//何もしない
}

bool ScenePause::IsLoaded() const
{
	//このシーンはロード関係をInitでする
	return true;
}

void ScenePause::Init()
{
	//フェードアウトをスキップする
	SkipFadeOut();
}

void ScenePause::End()
{
}

void ScenePause::Update()
{
	if(Input::GetInstance().IsTriggered("B"))
	{
		SceneManager::GetInstance().popScene();
		SkipFadeIn();
		return;
	}

	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}
}

void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0xffffff, false);

#ifdef _DEBUG
	DrawString(0, 16, "PAUSE", 0xffffff);
#endif
}
