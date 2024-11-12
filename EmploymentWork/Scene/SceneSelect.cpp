#include "SceneSelect.h"

#include "SceneTitle.h"
#include "SceneStageSelect.h"
#include "SceneStrengthen.h"
#include "SceneOption.h"
#include "SceneRanking.h"

namespace
{
#ifdef _DEBUG
	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
#endif
}

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
	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);
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


	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	DrawString(kTextX, kTextY, "ステージセレクト", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval, "強化", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 2, "オプション", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 3, "ランキング", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 4, "やめる", 0xffffff);
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
		if (m_destinationScene == eDestination::StageSelect)
		{
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneStageSelect>());
			EndThisScene();
			return;
		}
		else if(m_destinationScene == eDestination::Strengthen)
		{
			SceneManager::GetInstance().PushScene(std::make_shared<SceneStrengthen>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::Option)
		{
			SceneManager::GetInstance().PushScene(std::make_shared<SceneOption>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::Ranking)
		{
			SceneManager::GetInstance().PushScene(std::make_shared<SceneRanking>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::Quit)
		{
			SetIsGameEnd();
			return;
		}
	}

	//Bボタンを押したらタイトルに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}
}
