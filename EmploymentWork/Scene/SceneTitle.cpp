#include "SceneTitle.h"
#include "SceneStageSelect.h"
#include "SceneOption.h"

#include "Crystal.h"
#include "EnemyManager.h"
#include "Physics.h"

#include "Game.h"

#include "ResourceManager.h"
#include "MapManager.h"

namespace
{
//#ifdef _DEBUG	//デバッグ描画
	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
//#endif

	//ロゴ描画関係
	constexpr float kRogoSize = 0.55f;	//サイズ
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneTitle::SceneTitle():
	SceneBase("SCENE_TITLE"),
	m_enemyCreateFrame(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
SceneTitle::~SceneTitle()
{
	//staticクラスのデータのリセット
	ResourceManager::GetInstance().Clear(GetNowSceneName());
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneTitle::StartLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	//リソースデータ群をみてリソースのロードを開始する
	ResourceManager::GetInstance().Load(GetNowSceneName());

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneTitle::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する
	if (!ResourceManager::GetInstance().IsLoaded())	return false;

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneTitle::Init()
{
	//TODO:ここで実態の生成などをする
	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);

	m_rogoHandle = ResourceManager::GetInstance().GetHandle("I_ROGO");

	// ステージ情報をロード
	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load("title");

	m_pCrystal = std::make_shared<Crystal>(1);
	m_pCrystal->Init();
	m_pCrystal->Set(MapManager::GetInstance().GetCrystalPos());

	// ステージの当たり判定を設定
	MyLib::Physics::GetInstance().SetStageCollisionModel("M_STAGECOLLISIONTITLE", "M_STAGECOLLISIONTITLE");

	// 敵管理クラスの生成
	m_pEnemyManager = std::make_shared<EnemyManager>(false);
	m_pEnemyManager->LoadWayPoint("title");

	SetCameraNearFar(1.0f, 120.0f);
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 20.0f, -72.0f), VGet(0.0f, 0.0f, 0.0f));
	m_lightHandle = CreateDirLightHandle(VSub(VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 20.0f, -72.0f)));
}

/// <summary>
/// 終了
/// </summary>
void SceneTitle::End()
{
	//TODO:ここでリソースのメモリ開放などをする
	DeleteLightHandle(m_lightHandle);
}

/// <summary>
/// 更新
/// </summary>
void SceneTitle::Update()
{
	m_pCrystal->Update();
	m_pEnemyManager->Update(0, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f));
	m_pEnemyManager->CreateEnemy(0, m_enemyCreateFrame);

	// 物理更新
	MyLib::Physics::GetInstance().Update();
	m_pEnemyManager->UpdateModelPos();

	m_enemyCreateFrame++;
}

/// <summary>
/// 描画
/// </summary>
void SceneTitle::Draw()
{
	// リソースのロードが終わるまでは描画しないのがよさそう
	// (どちらにしろフェード仕切っているので何も見えないはず)
	if (!IsLoaded())	return;
	if (!IsInitialized())	return;

	// ステージの描画
	MapManager::GetInstance().Draw();
	m_pCrystal->Draw();
	m_pEnemyManager->Draw();

#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
	DrawString(kTextX, kTextY, "ゲームを始める", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval, "オプション", 0xffffff);
	DrawString(kTextX, kTextY + kTextYInterval * 2, "やめる", 0xffffff);
	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	//ロゴの描画
	DrawRotaGraph(Game::kWindowWidth / 2, Game::kWindowHeight / 4, kRogoSize, 0.0f, m_rogoHandle, true);
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneTitle::SelectNextSceneUpdate()
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
		if (m_destinationScene == eDestination::Select)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneStageSelect>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::Option)
		{
			SceneManager::GetInstance().SetNextScene(std::make_shared<SceneOption>());
			EndThisScene(true);
			return;
		}
		else if (m_destinationScene == eDestination::Quit)
		{
			SetIsGameEnd();
			return;
		}
	}
}
