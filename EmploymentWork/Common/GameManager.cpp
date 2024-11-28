#include "GameManager.h"

#include "Player.h"
#include "Camera.h"
#include "Crystal.h"
#include "HealPortion.h"
#include "SwarmEnemy.h"
#include "EnemyNormal.h"

#include "ModelManager.h"
#include "MapManager.h"
#include "TrapManager.h"
#include "EffectManager.h"
#include "LoadCSV.h"
#include "Input.h"

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
}

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager():
	m_isCreateEnemy(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager()
{
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
/// 初期化
/// </summary>
void GameManager::Init(int stageIdx)
{
	auto info = LoadCSV::GetInstance().LoadStageInfo(stageIdx);
	for (int i = 1; i < std::stoi(info[3]) + 1;i++)
	{
		m_phaseNum.push_back(-i);
		m_phaseNum.push_back(i);
	}

	//ステージの当たり判定モデルを取得する(描画するため)
	m_stageModel = ModelManager::GetInstance().GetModelHandle(info[1]);
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

	////クリスタルの生成
	//m_pCrystal = std::make_shared<Crystal>(10);
	//m_pCrystal->Init();
	//m_pCrystal->Set(Vec3(0.0f, 0.0f, 10.0f));

	//ステージ情報をロード
	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load(info[0].c_str());

	////DEBUG:ポーションを生成
	//m_pObjects.emplace_back(std::make_shared<HealPortion>());
	//m_pObjects.back()->Init();
	//m_pObjects.back()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));



	TrapManager::GetInstance().SetUp();
}

/// <summary>
/// 更新
/// </summary>
void GameManager::Update()
{
	if (Input::GetInstance().IsTriggered("Y"))
	{
		if (m_phaseNum.size() > 1)
		{
			m_phaseNum.pop_front();
		}
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

	if (m_phaseNum.front() >= 0)
	{
		if (!m_isCreateEnemy)
		{
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
		}
		m_isCreateEnemy = true;

	}
	else
	{
		m_isCreateEnemy = false;
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

	TrapManager::GetInstance().Update();

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
}

/// <summary>
/// 描画
/// </summary>
void GameManager::Draw()
{
	//ステージの描画
	MapManager::GetInstance().Draw();
	MV1DrawModel(m_stageModel);

	TrapManager::GetInstance().Draw();
#ifdef _DEBUG	//デバッグ描画
	MyLib::DebugDraw::Draw3D();
#endif

	//m_pCrystal->Draw();

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

	//TODO:UIクラスみたいなのを作ってそこに移動させる
	//装備スロットの描画
	for (int i = 0; i < 4; i++)
	{
		int x = 362 + i * 75;
		int y = 655;
		DrawBox(x - 30, y - 30, x + 30, y + 30, 0xffffff, false);
	}

	//現在選択しているスロット枠の描画
	DrawBox(362 + m_pPlayer->GetNowSlotNumber() * 75 - 35, 655 - 35, 362 + m_pPlayer->GetNowSlotNumber() * 75 + 35, 655 + 35, 0xff0000, false);


#ifdef _DEBUG	//デバッグ描画
	//クロスヘアの描画
	auto centerX = Game::kWindowWidth / 2;
	auto centerY = Game::kWindowHeight / 2;
	auto wid = 14;
	auto hei = 2;
	DrawBox(centerX - wid, centerY - hei, centerX + wid, centerY + hei, 0xffffff, true);
	DrawBox(centerX - hei, centerY - wid, centerX + hei, centerY + wid, 0xffffff, true);

	DrawFormatString(640, 0, 0xffffff, "フェーズ番号:%d", m_phaseNum.front());
#endif
}

/// <summary>
/// オブジェクトを追加する
/// </summary>
void GameManager::AddObject(std::shared_ptr<ObjectBase> pAddObject)
{
	m_pObjects.emplace_back(pAddObject);
}