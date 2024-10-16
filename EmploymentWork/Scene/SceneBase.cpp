#include "SceneBase.h"
#include "Game.h"

namespace
{
	// フェードにかかるフレームデフォルト
	constexpr int kFadeFrameDefault = 30;

	constexpr int kFadeSpeed = 255 / kFadeFrameDefault;

	//明るさの最大
	constexpr int kBrightMax = 255;
	//明るさの最低
	constexpr int kBrightMin = 0;
}

SceneBase::SceneBase() :
	m_fadeMask(-1),
	m_isInit(false),
	m_fadeBright(kBrightMax),
	m_fadeTotalFrame(kFadeFrameDefault),
	m_fadeSpeed(0),
	m_fadeColor(0x000000),
	m_fadePlayerKind(0),
	m_isDispLoading(false),
	m_loadFrame(0),
	m_isDispCursor(true),
	m_isEnd(false)
#ifdef DISP_PROCESS
	, m_updateTime(0),
	m_drawTime(0)
#endif
{
}

void SceneBase::EndThisScene()
{
	m_isEnd = true;
	StartFadeOut();
}

void SceneBase::InitAll()
{
	// フェードアウト状態から開始
	m_fadeBright = kBrightMax;
	StartFadeIn();
	// 継承先シーンの初期化処理
	Init();
}

void SceneBase::EndAll()
{
	// 継承先シーンの終了処理
	End();
}

void SceneBase::UpdateAll()
{
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
}

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

bool SceneBase::IsSceneEnd()
{
	// そもそも終わると言っていない
	if (!m_isEnd)	return false;
	// まだフェードアウト終わってない
	if (m_fadeBright < kBrightMax)	return false;

	return true;
}

void SceneBase::UpdateFade()
{
	// リソースのロードが完了していない
	if (m_fadeSpeed < 0 && (!IsLoaded()))
	{
		return;	// ロード完了まではフェードイン処理を行わない
	}

	m_fadeBright += m_fadeSpeed;
	if (m_fadeBright >= kBrightMax)
	{
		m_fadeBright = kBrightMax;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeBright <= kBrightMin)
	{
		m_fadeBright = kBrightMin;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

void SceneBase::DrawFade() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::DrawLoading() const
{
	if (!IsLoaded())
	{
		DrawFormatString(Game::kWindowWidth - 128, Game::kWindowHeight - 16,0xffffff,"Loading...");
	}
}

void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}
