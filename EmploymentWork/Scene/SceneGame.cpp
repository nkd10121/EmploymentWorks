#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"
#include "HealPortion.h"

#include "ModelManager.h"
#include "MapManager.h"
#include "LoadCSV.h"

#include "EnemyBase.h"
#include "EnemyNormal.h"

namespace
{
	//ポーションモデルのパス
	const std::string kPortionPath = "data/model/object/portion/bottle_red.mv1";
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneGame::SceneGame() :
	m_pPlayer(nullptr),
	m_pCamera(nullptr),
	m_pPhysics(nullptr),
	m_pObjects()
{
	//このシーンでロードするべきリソースのパスを取得
	m_loadPath = LoadCSV::GetInstance().GetLoadResourcePath("SCENE_GAME");
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

	for (auto& path : m_loadPath)
	{
		//モデルデータなら
		if (path.second == ".mv1")
		{
			auto p = path.first + path.second;
			ModelManager::GetInstance().LoadModel(p);
		}
	}


	//MapManager::GetInstance().LoadModel();

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

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneGame::Init()
{
	//TODO:ここで実態の生成などをする
	m_pPhysics = std::make_shared<MyLib::Physics>();

	//プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init(m_pPhysics);

	//カメラの生成
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	//ステージ情報をロード
	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load("data");

	//DEBUG:ポーションを生成
	m_pObjects.emplace_back(std::make_shared<HealPortion>());
	m_pObjects.back()->Init(m_pPhysics);
	m_pObjects.back()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));

	//DEBUG:敵を生成
	m_pEnemies.emplace_back(std::make_shared<EnemyNormal>());
	m_pEnemies.back()->Init(m_pPhysics);

	//ステージの当たり判定モデルを取得する(描画するため)
	m_stageModel = ModelManager::GetInstance().GetModelHandle("data/model/stage/Collision/Collision.mv1");
	MV1SetScale(m_stageModel, VGet(0.1f, 0.1f, 0.1f));
}

/// <summary>
/// 終了
/// </summary>
void SceneGame::End()
{
	//TODO:ここでリソースの開放をする

	MV1DeleteModel(m_stageModel);

	//ポーションの解放
	for (auto& object : m_pObjects)
	{
		object->Finalize(m_pPhysics);
	}
	m_pObjects.clear();
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

	////DEBUG:Aボタンを押した時にポーションを生成するように
	//if (Input::GetInstance().IsTriggered("A"))
	//{
	//	m_pObjects.emplace_back(std::make_shared<HealPortion>());
	//	m_pObjects.back()->Init(m_pPhysics);
	//	m_pObjects.back()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
	//}

	//プレイヤーの更新
	m_pPlayer->SetCameraAngle(m_pCamera->GetDirection());
	m_pPlayer->Update(this);

	//敵の更新
	for (auto& enemy : m_pEnemies)
	{
		enemy->Update();
		if (!enemy->GetIsExist())
		{
			enemy->Finalize();
		}
	}
	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_pEnemies.begin(), m_pEnemies.end(), [](auto& v)
			{
				return v->GetIsExist() == false;
			});
		m_pEnemies.erase(it, m_pEnemies.end());
	}

	//ポーションの更新
	for (auto& object : m_pObjects)
	{
		object->Update();

		if (!object->GetIsExist())
		{
			object->Finalize(m_pPhysics);
		}
	}
	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_pObjects.begin(), m_pObjects.end(), [](auto& v)
			{
				return v->GetIsExist() == false;
			});
		m_pObjects.erase(it, m_pObjects.end());
	}

	//物理更新
	m_pPhysics->Update();

	//カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->Update();
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

	//ステージの描画
	//MapManager::GetInstance().Draw();
	MV1DrawModel(m_stageModel);


	//プレイヤーの描画
	m_pPlayer->Draw();

	for (auto& enemy : m_pEnemies)
	{
		enemy->Draw();
	}

	//ポーションの描画
	for (auto& object : m_pObjects)
	{
		object->Draw();
	}

#ifdef _DEBUG
	MyLib::DebugDraw::Draw3D();

	DrawString(0, 0, "GAME", 0xffffff);
#endif
}

void SceneGame::AddObject(std::shared_ptr<ObjectBase> pAddObject)
{
	m_pObjects.emplace_back(pAddObject);
}
