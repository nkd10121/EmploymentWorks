#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"
#include "Crystal.h"
#include "HealPortion.h"

#include "ModelManager.h"
#include "TrapManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "MapManager.h"
#include "LoadCSV.h"
#include "Game.h"

#include "SwarmEnemy.h"
#include "EnemyNormal.h"

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
	m_pPlayer(nullptr),
	m_pCamera(nullptr),
	m_pPhysics(nullptr),
	m_pObjects(),
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

	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneGame::Init()
{
	//TODO:ここで実態の生成などをする

	//ステージの当たり判定モデルを取得する(描画するため)
	m_stageModel = ModelManager::GetInstance().GetModelHandle("M_STAGECOLLISION");
	MV1SetScale(m_stageModel, VGet(0.01f, 0.01f, 0.01f));		//サイズの変更
	MV1SetRotationXYZ(m_stageModel, VGet(0.0f, DX_PI_F, 0.0f));	//回転

	//ステージの当たり判定を設定
	MyLib::Physics::GetInstance().SetStageCollisionModel(m_stageModel);

	//プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init();

	//カメラの生成
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init(m_stageModel);

	//クリスタルの生成
	m_pCrystal = std::make_shared<Crystal>(10);
	m_pCrystal->Init();
	m_pCrystal->Set(Vec3(0.0f, 0.0f, 10.0f));

	//ステージ情報をロード
	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load("test");

	//DEBUG:ポーションを生成
	m_pObjects.emplace_back(std::make_shared<HealPortion>());
	m_pObjects.back()->Init();
	m_pObjects.back()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));

	//DEBUG:敵を生成
	for (int i = 0; i < 1; i++)
	{
		auto addSwarm = std::make_shared<SwarmEnemy>(kColor[i]);

		for (int j = 0; j < 1; j++)
		{
			auto add = std::make_shared<EnemyNormal>();
			add->SetPos(Vec3(-48.0f + 16 * i, 8.0f, -48.0f + 16 * j));
			add->Init();

			addSwarm->AddSwarm(add);

		}

		addSwarm->SetUp();
		m_pEnemies.emplace_back(addSwarm);
	}

	TrapManager::GetInstance().SetUp();

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

	MV1DeleteModel(m_stageModel);

	m_pPlayer->Finalize();

	for (auto& enemy : m_pEnemies)
	{
		enemy->Finalize();
	}
	m_pEnemies.clear();

	//ポーションの解放
	for (auto& object : m_pObjects)
	{
		object->Finalize();
	}
	m_pObjects.clear();

	MyLib::Physics::GetInstance().Clear();

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

	//ステージの描画
	MapManager::GetInstance().Draw();
	MV1DrawModel(m_stageModel);

#ifdef _DEBUG	//デバッグ描画
	TrapManager::GetInstance().Draw();
	MyLib::DebugDraw::Draw3D();
#endif

	m_pCrystal->Draw();

	EffectManager::GetInstance().Draw();

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

	(this->*m_drawFunc)();

#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());

	//クロスヘアの描画
	auto centerX = Game::kWindowWidth / 2;
	auto centerY = Game::kWindowHeight / 2;
	auto wid = 14;
	auto hei = 2;
	DrawBox(centerX - wid, centerY - hei, centerX + wid, centerY + hei, 0xffffff, true);
	DrawBox(centerX - hei, centerY - wid, centerX + hei, centerY + wid, 0xffffff, true);

	DrawFormatString(1120, 0, 0xbbbbbb, "クリスタルHP:%d", m_pCrystal->GetHp());
#endif
}

/// <summary>
/// オブジェクトを追加する
/// ほかのクラスからゲームシーンで管理したいオブジェクトを追加するときに使用する
/// </summary>
/// <param name="pAddObject"></param>
void SceneGame::AddObject(std::shared_ptr<ObjectBase> pAddObject)
{
	m_pObjects.emplace_back(pAddObject);
}

void SceneGame::UpdateGame()
{
	m_pCrystal->Update();
	if (m_pCrystal->GetIsBreak())
	{
		DrawFormatString(640, 0, 0xffff00, "ゲームオーバー");
	}

	//プレイヤーの更新
	m_pPlayer->SetCameraAngle(m_pCamera->GetDirection());
	m_pPlayer->Update(this);
	if (m_pPlayer->GetIsDeath())
	{
		//プレイヤーの生成
		m_pPlayer = std::make_shared<Player>();
		m_pPlayer->Init();
	}

	//敵の更新
	for (auto& enemy : m_pEnemies)
	{
		enemy->Update();
	}
	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_pEnemies.begin(), m_pEnemies.end(), [](auto& v)
			{
				return v->GetIsExistMember() == false;
			});
		m_pEnemies.erase(it, m_pEnemies.end());
	}

	//ポーションの更新
	for (auto& object : m_pObjects)
	{
		object->Update();

		if (!object->GetIsExist())
		{
			object->Finalize();
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
	MyLib::Physics::GetInstance().Update();

	//モデル座標の更新
	m_pPlayer->UpdateModelPos();

	for (auto& enemy : m_pEnemies)
	{
		enemy->UpdateModelPos();
	}

	//カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->Update();

	//エフェクトの更新
	EffectManager::GetInstance().Update();

	if (Input::GetInstance().IsTriggered("Y"))
	{
		m_updateFunc = &SceneGame::UpdateTrapSelect;
		m_drawFunc = &SceneGame::DrawTrapSelect;
	}
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
