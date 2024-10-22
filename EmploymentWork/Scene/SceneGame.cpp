#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneGame::SceneGame():
	temp_handle(),
	m_pPhysics(nullptr)
{
	m_pPlayer = std::make_shared<Player>();

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
		temp_handle.push_back(MV1LoadModel("data/model/temp_stage1.mv1"));
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
	for (auto& h : temp_handle)
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
	for (auto& h : temp_handle)
	{
		MV1SetScale(h, VGet(0.01f, 0.01f, 0.01f));
	}

	m_pPhysics = std::make_shared<MyLib::Physics>();
	m_pPlayer->Init(m_pPhysics);

	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();


}

/// <summary>
/// 終了
/// </summary>
void SceneGame::End()
{
	//TODO:ここでリソースのメモリ開放などをする
	for (auto& h : temp_handle)
	{
		MV1DeleteModel(h);
	}
	temp_handle.clear();
}

/// <summary>
/// 更新
/// </summary>
void SceneGame::Update()
{
#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}

	if (Input::GetInstance().IsTriggered("Y"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}

	m_pCamera->Update();

	m_pPlayer->Update();

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

	for (auto& h : temp_handle)
	{
		MV1DrawModel(h);
	}

	m_pPlayer->Draw();

#ifdef _DEBUG
	MyLib::DebugDraw::Draw3D();

	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
