#include "SceneBase.h"
#include "Game.h"

#include "ModelManager.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "MapManager.h"

namespace
{
	// フェードにかかるフレームデフォルト
	constexpr int kFadeFrameDefault = 30;
	// 1フレーム当たりのフェード速度
	constexpr int kFadeSpeed = 255 / kFadeFrameDefault;

	//明るさの最大
	constexpr int kBrightMax = 255;
	//明るさの最低
	constexpr int kBrightMin = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneBase::SceneBase() :
	m_isInit(false),
	m_isEnd(false),
	m_fadeAlpha(kBrightMax),
	m_fadeSpeed(0),
	m_fadeColor(0x000000)
#ifdef DISP_PROCESS
	, m_updateTime(0),
	m_drawTime(0)
#endif
{
}

/// <summary>
/// デストラクタ
/// </summary>
void SceneBase::EndThisScene()
{
	m_isEnd = true;
	StartFadeOut();
}

/// <summary>
/// 派生先の初期化とシーン共通で必要な初期化を行う
/// </summary>
void SceneBase::InitAll()
{
	// フェードアウト状態から開始
	m_fadeAlpha = kBrightMax;
	StartFadeIn();
	// 継承先シーンの初期化処理
	Init();
}

/// <summary>
/// 派生先の更新とシーン共通で必要な更新を行う
/// </summary>
void SceneBase::UpdateAll()
{
#ifdef DISP_PROCESS
	LONGLONG start = GetNowHiPerformanceCount();
#endif
	//継承先のシーンのリソースのロードが終わっているか確認
	if (!IsLoaded())
	{
		//ロード中なら更新処理何も行わない
		return;
	}

	//初期化関数を呼んだか確認
	if (!m_isInit)
	{
		//呼んでいなかったら初期化関数を呼ぶ
		InitAll();
		m_isInit = true;
	}

	UpdateFade();
	// 継承先のシーンの更新処理
	Update();

#ifdef DISP_PROCESS
	m_updateTime = GetNowHiPerformanceCount() - start;
#endif
}

/// <summary>
/// 派生先の描画とシーン共通で必要な描画を行う
/// </summary>
void SceneBase::DrawAll()
{
#ifdef DISP_PROCESS
	LONGLONG start = GetNowHiPerformanceCount();
#endif

#ifdef TRUE
	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}
#endif

	//継承先のシーンの描画処理
	Draw();
	DrawFade();
	DrawLoading();

#ifdef DISP_PROCESS
	m_drawTime = GetNowHiPerformanceCount() - start;

	// 処理バーの表示
	// 説明 
	DrawString(0, Game::kWindowHeight - 48, "処理", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kWindowHeight - 48 + 2, 48 + 16 - 2, Game::kWindowHeight - 32 - 2, 0x0000ff, true);
	DrawString(0, Game::kWindowHeight - 32, "描画", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kWindowHeight - 32 + 2, 48 + 16 - 2, Game::kWindowHeight - 16 - 2, 0xff0000, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	int width = static_cast<int>(Game::kWindowWidth * rate);
	DrawBox(0, Game::kWindowHeight - 16, width, Game::kWindowHeight, 0xff0000, true);	// 処理+描画

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<int>(Game::kWindowWidth * rate);
	DrawBox(0, Game::kWindowHeight - 16, width, Game::kWindowHeight, 0x0000ff, true);	// 処理+描画の上から処理時間のみ描画
#endif
}

/// <summary>
/// 現在のシーンが完全に終了したかどうか
/// </summary>
bool SceneBase::IsSceneEnd()
{
	// そもそも終わると言っていない
	if (!m_isEnd)	return false;
	// まだフェードアウト終わってない
	if (m_fadeAlpha < kBrightMax)	return false;


	ModelManager::GetInstance().Clear();
	SoundManager::GetInstance().Clear();
	EffectManager::GetInstance().Clear();
	MapManager::GetInstance().DeleteModel();
	return true;
}

/// <summary>
/// フェードの更新
/// </summary>
void SceneBase::UpdateFade()
{
	// リソースのロードが完了していない
	if (m_fadeSpeed < 0 && (!IsLoaded()))
	{
		return;	// ロード完了まではフェードイン処理を行わない
	}

	m_fadeAlpha += m_fadeSpeed;
	if (m_fadeAlpha >= kBrightMax)
	{
		m_fadeAlpha = kBrightMax;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeAlpha <= kBrightMin)
	{
		m_fadeAlpha = kBrightMin;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

/// <summary>
/// フェードの描画
/// </summary>
void SceneBase::DrawFade() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ロード中描画
/// </summary>
void SceneBase::DrawLoading() const
{
	if (!IsLoaded())
	{
		DrawFormatString(Game::kWindowWidth - 128, Game::kWindowHeight - 16,0xffffff,"Loading...");
	}
}

/// <summary>
/// フェードイン開始
/// </summary>
void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

/// <summary>
/// フェードアウト開始
/// </summary>
void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}

/// <summary>
/// フェードインをスキップする
/// </summary>
void SceneBase::SkipFadeIn()
{
	m_fadeAlpha = kBrightMax;
	m_fadeSpeed = kFadeSpeed;
}

/// <summary>
/// フェードアウトをスキップする
/// </summary>
void SceneBase::SkipFadeOut()
{
	m_fadeAlpha = kBrightMin;
	m_fadeSpeed = -kFadeSpeed;
}
