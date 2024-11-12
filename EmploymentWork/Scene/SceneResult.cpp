#include "SceneResult.h"

#include "SceneGame.h"
#include "SceneStrengthen.h"
#include "SceneRanking.h"
#include "SceneMainMenu.h"

namespace
{
#ifdef _DEBUG	//デバッグ描画
	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
#endif
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneResult::SceneResult():
	SceneBase("SCENE_RESULT")
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneResult::~SceneResult()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneResult::StartLoad()
{
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneResult::IsLoaded() const
{
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneResult::Init()
{
	//最初は一番上の項目を選んでいる状態にする
	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);
}

/// <summary>
/// 終了
/// </summary>
void SceneResult::End()
{
}

/// <summary>
/// 更新
/// </summary>
void SceneResult::Update()
{
}

/// <summary>
/// 描画
/// </summary>
void SceneResult::Draw()
{
#ifdef _DEBUG	//デバッグ描画	
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());

	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	DrawString(kTextX, kTextY, "次のステージへ", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval, "スコア詳細へ", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 2, "強化へ", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 3, "ランキングへ", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 4, "メインメニューに戻る", 0xffffff);
#endif
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneResult::SelectNextSceneUpdate()
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
		//ゲームシーンに遷移する
		if (m_destinationScene == eDestination::InGame)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneGame>());
			EndThisScene();
			return;
		}
		//スコア詳細が出てくる
		else if (m_destinationScene == eDestination::ScoreDetail)
		{

		}
		//強化シーンに遷移する
		else if (m_destinationScene == eDestination::Strengthen)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneStrengthen>());
			EndThisScene();
			return;
		}
		//ランキングシーンに遷移する
		else if (m_destinationScene == eDestination::Ranking)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneRanking>());
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
	}
}
