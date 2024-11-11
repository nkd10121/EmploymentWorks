#include "SceneSelect.h"

SceneSelect::SceneSelect() :
	SceneBase("SCENE_SELECT")
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::StartLoad()
{
}

bool SceneSelect::IsLoaded() const
{
	return true;
}

void SceneSelect::Init()
{
}

void SceneSelect::End()
{
}

void SceneSelect::Update()
{
}

void SceneSelect::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneSelect::SelectNextSceneUpdate()
{
	//上を入力したら
	if (Input::GetInstance().IsTriggered("UP"))
	{
		//現在選択している項目から一個上にずらす
		m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) - 1);

		//もし一番上の項目を選択している状態になっていたら
		if (m_destinationScene == eDestination::Start)
		{
			//一個下にずらす
			m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) + 1);
		}
	}

	//下を入力したら
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		//現在選択している項目から一個下にずらす
		m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) + 1);

		//もし一番下の項目を選択している状態になっていたら
		if (m_destinationScene == eDestination::Last)
		{
			//一個上にずらす
			m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) - 1);
		}
	}

	//決定ボタンを押したら現在選択しているシーンに遷移する
	if (Input::GetInstance().IsTriggered("OK"))
	{
		////ゲームシーンに遷移する
		//if (m_destinationScene == eDestination::StageSelect)
		//{
		//	SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		//	EndThisScene();
		//	return;
		//}
	}
}
