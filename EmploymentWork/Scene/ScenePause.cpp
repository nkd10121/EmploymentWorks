#include "ScenePause.h"
#include "Game.h"
#include "SceneTitle.h"

/// <summary>
/// コンストラクタ
/// </summary>
ScenePause::ScenePause():
	SceneBase("SCENE_PAUSE")
{
}

/// <summary>
/// デストラクタ
/// </summary>
ScenePause::~ScenePause()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void ScenePause::StartLoad()
{
	//何もしない
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool ScenePause::IsLoaded() const
{
	//このシーンはロード関係をInitでする
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void ScenePause::Init()
{
	//フェードアウトをスキップする
	SkipFadeOut();
}

/// <summary>
/// 終了
/// </summary>
void ScenePause::End()
{
}

/// <summary>
/// 更新
/// </summary>
void ScenePause::Update()
{
	if(Input::GetInstance().IsTriggered("CANSEL"))
	{
		SceneManager::GetInstance().PopScene();
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

/// <summary>
/// 描画
/// </summary>
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
