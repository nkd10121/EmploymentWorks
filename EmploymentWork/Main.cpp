#include "DxLib.h"
#include "Game.h"
#include "Camera.h"
#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(true);
	//ウィンドウサイズの変更
	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 32);
	// ウインドウ名設定
	SetMainWindowText(Game::kWindowName);
	//フルスクリーン切り替え時にリソースハンドルをリセットしないように設定する
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	//カメラの生成
	std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

		m_pCamera->Update();

		for (int x = -50; x <= 50; x += 10)
		{
			DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
		}
		for (int z = -50; z <= 50; z += 10)
		{
			DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
		}

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}