#include "ScenePause.h"
#include "Game.h"
#include "SceneTitle.h"
#include "SceneGame.h"

namespace
{
	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
}

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

	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);
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
	if(Input::GetInstance().IsTriggered("OK"))
	{
		//タイトルシーンに遷移する
		if (m_destinationScene == eDestination::InGame)
		{
			SceneManager::GetInstance().PopScene();
			SkipFadeIn();
			return;
		}
		//ゲームシーンに遷移する
		else if (m_destinationScene == eDestination::ReStart)
		{
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
			EndThisScene();
			return;
		}
		//ポーズシーンを上に表示する
		else if (m_destinationScene == eDestination::Title)
		{
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
			EndThisScene();
			return;
		}
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

	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	DrawString(kTextX, kTextY, "ゲームに戻る", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval, "リスタート", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 2, "タイトルに戻る", 0xffffff);
#endif
}
