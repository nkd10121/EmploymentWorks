#include "SceneStageSelect.h"

#include "SceneGame.h"
#include "SceneMainMenu.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneStageSelect::SceneStageSelect():
	SceneBase("SCENE_STAGESELECT")
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneStageSelect::StartLoad()
{
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneStageSelect::IsLoaded() const
{
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneStageSelect::Init()
{
}

/// <summary>
/// 終了
/// </summary>
void SceneStageSelect::End()
{
}

/// <summary>
/// 更新
/// </summary>
void SceneStageSelect::Update()
{
}

/// <summary>
/// 描画
/// </summary>
void SceneStageSelect::Draw()
{
#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneStageSelect::SelectNextSceneUpdate()
{
	//決定ボタンを押したら現在選択しているシーンに遷移する
	if (Input::GetInstance().IsTriggered("OK"))
	{
		//ゲームシーンに遷移する
		SceneManager::GetInstance().SetNextScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}

	//Bボタンを押したらセレクトシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		SceneManager::GetInstance().SetNextScene(std::make_shared<SceneMainMenu>());
		EndThisScene();
		return;
	}
}