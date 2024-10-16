#include "SceneTitle.h"
#include "SceneGame.h"

SceneTitle::SceneTitle()
{

}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::StartLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	//TODO:この間でリソースをロードする

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneTitle::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する
	//if (CheckHandleASyncLoad(m_bgTile))	return false;

	return true;
}

void SceneTitle::Init()
{
	//TODO:ここで実態の生成などをする
}

void SceneTitle::End()
{
	//TODO:ここでリソースのメモリ開放などをする
}

void SceneTitle::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}
}

void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TITLE", 0xffffff);
#endif
}
