#include "SceneTitle.h"
#include "SceneGame.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneTitle::SceneTitle():
	SceneBase("SCENE_TITLE")
{

}

/// <summary>
/// デストラクタ
/// </summary>
SceneTitle::~SceneTitle()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneTitle::StartLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	//TODO:この間でリソースをロードする

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneTitle::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する
	//if (CheckHandleASyncLoad(m_bgTile))	return false;

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneTitle::Init()
{
	//TODO:ここで実態の生成などをする
}

/// <summary>
/// 終了
/// </summary>
void SceneTitle::End()
{
	//TODO:ここでリソースのメモリ開放などをする
}

/// <summary>
/// 更新
/// </summary>
void SceneTitle::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}
}

/// <summary>
/// 描画
/// </summary>
void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TITLE", 0xffffff);
#endif
}
