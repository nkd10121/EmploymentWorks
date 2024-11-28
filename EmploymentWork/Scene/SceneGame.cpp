#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"

#include "ModelManager.h"
#include "SoundManager.h"
#include "ImageManager.h"
#include "GameManager.h"

#include "Game.h"


namespace
{
	const unsigned int kColor[6]
	{
		0xffffff,
		0x000000,
		0xff0000,
		0x00ff00,
		0x0000ff,
		0xffff00
	};

	/*トラップセレクト関係*/
	constexpr int kBoxSize = 40;
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneGame::SceneGame() :
	SceneBase("SCENE_GAME"),
	m_nowCursor(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
SceneGame::~SceneGame()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneGame::StartLoad()
{
	// TODO:この間でリソースをロードする

	//このシーンでロードするべきリソースのデータを取得
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
bool SceneGame::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する

	if (!ModelManager::GetInstance().IsLoaded())	return false;
	if (!SoundManager::GetInstance().IsLoaded())	return false;
	if (!ImageManager::GetInstance().IsLoaded())	return false;

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneGame::Init()
{
	//TODO:ここで実態の生成などをする
	m_pGameManager = std::make_shared<GameManager>();
	m_pGameManager->Init(0);

	//通常状態に設定しておく
	m_updateFunc = &SceneGame::UpdateGame;
	m_drawFunc = &SceneGame::DrawNormal;
}

/// <summary>
/// 終了
/// </summary>
void SceneGame::End()
{
	//TODO:ここでリソースの開放をする

}

/// <summary>
/// 更新
/// </summary>
void SceneGame::Update()
{
#ifdef _DEBUG	//デバッグ描画
	MyLib::DebugDraw::Clear();
#endif
	////Xボタンを押したらタイトル画面に戻るように
	//if (Input::GetInstance().IsTriggered("X"))
	//{
	//	SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
	//	EndThisScene();
	//	return;
	//}

	//Yボタンを押したらポーズ画面を開くように
	if (Input::GetInstance().IsTriggered("PAUSE"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}

	//状態の更新
	(this->*m_updateFunc)();

	////DEBUG:Aボタンを押した時にポーションを生成するように
	//if (Input::GetInstance().IsTriggered("Y"))
	//{
	//	m_pEnemies.emplace_back(std::make_shared<EnemyNormal>());
	//	m_pEnemies.back()->Init(m_pPhysics);
	//}
}

/// <summary>
/// 描画
/// </summary>
void SceneGame::Draw()
{
	// リソースのロードが終わるまでは描画しないのがよさそう
	// (どちらにしろフェード仕切っているので何も見えないはず)
	if (!IsLoaded())	return;
	if (!IsInitialized())	return;

	
	m_pGameManager->Draw();


	(this->*m_drawFunc)();



#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
}

void SceneGame::UpdateGame()
{
	m_pGameManager->Update();
}

void SceneGame::UpdateTrapSelect()
{
	if (Input::GetInstance().IsTriggered("Y"))
	{
		m_updateFunc = &SceneGame::UpdateGame;
		m_drawFunc = &SceneGame::DrawNormal;
	}
}

void SceneGame::DrawNormal()
{
	//処理なし
}

void SceneGame::DrawTrapSelect()
{
	//大枠
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0xffffff, false);

	//枠
	for (int x = 100; x < Game::kWindowWidth; x += 120)
	{
		int y = Game::kWindowHeight - 50 - kBoxSize;
		DrawBox(x - kBoxSize, y - kBoxSize, x + kBoxSize, y + kBoxSize, 0xffffff, false);
	}

	DrawBox(100 + m_nowCursor * 120 - 55, 630 - 55, 100 + m_nowCursor * 120 + 55, 630 + 55, 0xff0000, false);

	if (Input::GetInstance().IsTriggered("RIGHT"))
	{
		m_nowCursor++;

		if (m_nowCursor > 9)
		{
			m_nowCursor = 9;
		}
	}
	else if (Input::GetInstance().IsTriggered("LEFT"))
	{
		m_nowCursor--;

		if (m_nowCursor < 0)
		{
			m_nowCursor = 0;
		}
	}
}
