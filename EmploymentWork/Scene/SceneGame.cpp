#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"
#include "ModelManager.h"

namespace
{
	//ステージパス
	const std::string stagePath = "data/model/temp_stage1.mv1";
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneGame::SceneGame():
	m_pPlayer(nullptr),
	m_pCamera(nullptr),
	m_pPhysics(nullptr),
	m_modelHandles()
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
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	// TODO:この間でリソースをロードする

	for (int i = 0; i < 1; i++)
	{
		ModelManager::GetInstance().LoadModel(stagePath);
	}

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneGame::IsLoaded() const
{
	//TODO:ここでリソースがロード中かどうかを判断する

	auto modelHandle = ModelManager::GetInstance().GetPrimitiveModelHandles();
	for (auto& h : modelHandle)
	{
		if (CheckHandleASyncLoad(h))	return false;
	}

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneGame::Init()
{
	//TODO:ここで実態の生成などをする

	m_pPlayer = std::make_shared<Player>();

	m_pPhysics = std::make_shared<MyLib::Physics>();
	m_pPlayer->Init(m_pPhysics);

	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	m_modelHandles.emplace_back(ModelManager::GetInstance().GetModelHandle(stagePath));
	MV1SetScale(m_modelHandles.back(), VGet(0.01f, 0.01f, 0.01f));
}

/// <summary>
/// 終了
/// </summary>
void SceneGame::End()
{
	//TODO:ここでリソースの開放をする

	for (auto& h : m_modelHandles)
	{
		MV1DeleteModel(h);
	}
	m_modelHandles.clear();
}

/// <summary>
/// 更新
/// </summary>
void SceneGame::Update()
{
#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif
	//Xボタンを押したらタイトル画面に戻るように
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}

	//Yボタンを押したらポーズ画面を開くように
	if (Input::GetInstance().IsTriggered("Y"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}

	//プレイヤーの更新
	m_pPlayer->SetCameraAngle(m_pCamera->GetDirection());
	m_pPlayer->Update();

	//カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->Update();



	// 物理更新
	m_pPhysics->Update();
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

	for (auto& h : m_modelHandles)
	{
		MV1DrawModel(h);
	}

	m_pPlayer->Draw();

#ifdef _DEBUG
	MyLib::DebugDraw::Draw3D();

	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
