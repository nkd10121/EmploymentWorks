#include "SceneDebug.h"

#include "SceneTitle.h"
#include "SceneMainMenu.h"
#include "SceneStageSelect.h"
#include "SceneStrengthen.h"
#include "SceneRanking.h"
#include "SceneGame.h"
#include "SceneResult.h"
#include "ScenePause.h"
#include "SceneOption.h"

#include "SoundManager.h"
#include "LoadCSV.h"

namespace
{
	const std::string kSceneName[] =
	{
		"SceneTitle",
		"SceneMainMenu",
		"SceneStageSelect",
		"SceneStrengthen",
		"SceneRanking",
		"SceneGame",
		"SceneResult",
		"ScenePause",
		"SceneOption",
	};

	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneDebug::SceneDebug() :
	SceneBase("SCENE_DEBUG")
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneDebug::~SceneDebug()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneDebug::StartLoad()
{
	//このシーンでロードするべきリソースのパスを取得
	auto loadResourceData = LoadCSV::GetInstance().GetLoadResourcePath(GetNowSceneName());

	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	//リソースデータ群をみてリソースのロードを開始する
	AssortAndLoadResourse(loadResourceData);

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneDebug::IsLoaded() const
{
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneDebug::Init()
{
	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);
}

/// <summary>
/// 終了
/// </summary>
void SceneDebug::End()
{
}

/// <summary>
/// 更新
/// </summary>
void SceneDebug::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void SceneDebug::Draw()
{
	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	int y = kTextY;
	for (auto& name : kSceneName)
	{
		DrawString(kTextX,y,name.c_str(),0xffffff);

		y += kTextYInterval;
	}

#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff,"%s", GetNowSceneName());
#endif
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneDebug::SelectNextSceneUpdate()
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
		//タイトルシーンに遷移する
		if (m_destinationScene == eDestination::Title)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneTitle>());
			EndThisScene();
			return;
		}
		//セレクトシーンに遷移する
		else if (m_destinationScene == eDestination::Select)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneMainMenu>());
			EndThisScene();
			return;
		}
		//ステージセレクトシーンに遷移する
		else if (m_destinationScene == eDestination::StageSelect)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneStageSelect>());
			EndThisScene();
			return;
		}
		//強化シーンに遷移する
		else if (m_destinationScene == eDestination::Strengthen)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneStrengthen>());
			EndThisScene(true);
			return;
		}
		//強化シーンに遷移する
		else if (m_destinationScene == eDestination::Ranking)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneRanking>());
			EndThisScene(true);
			return;
		}
		//ゲームシーンに遷移する
		else if (m_destinationScene == eDestination::InGame)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneGame>());
			EndThisScene();
			return;
		}
		//ゲームシーンに遷移する
		else if (m_destinationScene == eDestination::Result)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneResult>());
			EndThisScene(true);
			return;
		}
		//ポーズシーンを上に表示する
		else if (m_destinationScene == eDestination::Pause)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<ScenePause>());
			EndThisScene(true);
			return;
		}
		//オプションシーンに遷移する
		else if (m_destinationScene == eDestination::Option)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneOption>());
			EndThisScene(true);
			return;
		}
	}
}
