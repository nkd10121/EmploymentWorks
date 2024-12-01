#include "GameManager.h"

#include "Player.h"
#include "Camera.h"
#include "Crystal.h"
#include "HealPortion.h"
#include "EnemyManager.h"

#include "ModelManager.h"
#include "MapManager.h"
#include "TrapManager.h"
#include "EffectManager.h"
#include "ImageManager.h"
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
GameManager::GameManager() :
	m_isCreateEnemy(false),
	m_phaseCount(0)
{

}

/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager()
{
	MV1DeleteModel(m_stageModel);

	m_pPlayer->Finalize();

	m_pEnemyManager->Finalize();

	//ポーションの解放
	for (auto& object : m_pObjects)
	{
		object->Finalize();
	}
	m_pObjects.clear();

	MyLib::Physics::GetInstance().Clear();

	DeleteGraph(m_slotBgHandle);
	for (auto& h : m_slotIconHandle)
	{
		DeleteGraph(h);
	}
}

/// <summary>
/// 初期化
/// </summary>
void GameManager::Init(int stageIdx)
{
	auto info = LoadCSV::GetInstance().LoadStageInfo(stageIdx);
	for (int i = 1; i < std::stoi(info[3]) + 1; i++)
	{
		m_phaseNum.push_back(-i);
		m_phaseNum.push_back(i);
	}
	//クリア番号として0を最後に入れておく
	m_phaseNum.push_back(0);

	//ステージの当たり判定モデルを取得する(描画するため)
	m_stageModel = ModelManager::GetInstance().GetModelHandle(info[1]);
	MV1SetScale(m_stageModel, VGet(0.01f, 0.01f, 0.01f));		//サイズの変更
	MV1SetRotationXYZ(m_stageModel, VGet(0.0f, DX_PI_F, 0.0f));	//回転

	//ステージの当たり判定を設定
	MyLib::Physics::GetInstance().SetStageCollisionModel(m_stageModel);

	//プレイヤーの生成
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init();

	//敵管理クラスの生成
	m_pEnemyManager = std::make_shared<EnemyManager>();
	m_pEnemyManager->Init(info[0].c_str());

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

	m_slotBgHandle = ImageManager::GetInstance().GetHandle("I_SLOTBG");
	m_slotIconHandle.push_back(ImageManager::GetInstance().GetHandle("I_SLOTCROSSBOW"));
	m_slotIconHandle.push_back(ImageManager::GetInstance().GetHandle("I_CRYSTALBG"));
	m_slotIconHandle.push_back(ImageManager::GetInstance().GetHandle("I_IRONUI"));
	m_slotIconHandle.push_back(ImageManager::GetInstance().GetHandle("I_MINIMAPBG"));

	TrapManager::GetInstance().SetUp();
}

/// <summary>
/// 更新
/// </summary>
void GameManager::Update()
{
	//Yボタンを押した時かつ最初のフェーズの時、
	if (Input::GetInstance().IsTriggered("Y") && m_phaseNum.front() == -1)
	{
		//次のフェーズに進む
		m_phaseNum.pop_front();
	}

	//カメラの更新
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->Update();
	auto hitPos = m_pCamera->GetMapHitPosition();

	//現在のフェーズが0以上(戦闘フェーズ)の時、敵を生成していなかったら敵を生成する
	if (m_phaseNum.front() >= 0)
	{
		if (!m_isCreateEnemy)
		{
			m_pEnemyManager->CreateEnemy(m_phaseNum.front());
		}
		//敵を生成した
		m_isCreateEnemy = true;

	}
	//0以下(準備フェーズ)の時
	else
	{
		//敵生成フラグをリセットする
		m_isCreateEnemy = false;
	}

	//敵の更新処理
	auto isNextPhase = m_pEnemyManager->Update(m_phaseNum.front(),m_pCamera->GetCameraPos(), m_pCamera->GetDirection());
	Vec3 rayCastRet;
	auto cameraToEnemy = (m_pEnemyManager->GetRayCastRetPos() - m_pCamera->GetCameraPos()).Length();
	auto cameraToMap = (m_pCamera->GetMapHitPosition() - m_pCamera->GetCameraPos()).Length();
	if (cameraToEnemy > cameraToMap)
	{
		rayCastRet = m_pCamera->GetMapHitPosition();
	}
	else
	{
		if (m_pEnemyManager->GetRayCastRetPos().Length() != 0.0f)
		{
			rayCastRet = m_pEnemyManager->GetRayCastRetPos();
		}
		else
		{
			rayCastRet = m_pCamera->GetMapHitPosition();
		}
	}

	//プレイヤーの更新
	m_pPlayer->SetCameraAngle(m_pCamera->GetDirection());
	m_pPlayer->Update(this, rayCastRet);
	if (m_pPlayer->GetIsDeath())
	{
		//プレイヤーの生成
		m_pPlayer = std::make_shared<Player>();
		m_pPlayer->Init();
	}

	//敵が全滅した時、次のフェーズに進む
	if (isNextPhase)
	{
		//念のため、現在が戦闘フェーズであるか確認
		if (m_phaseNum.front() > 0)
		{
			//次のフェーズに進む
			m_phaseNum.pop_front();
		}
	}

	//最初の準備フレームでなければフェーズカウントを進める
	if (m_phaseNum.front() < 0 && m_phaseNum.front() != -1)
	{
		m_phaseCount++;
	}

	//10秒経ったら次のフェーズに進める
	if (m_phaseCount >= 600)
	{
		m_phaseNum.pop_front();
		//フェーズカウントをリセット
		m_phaseCount = 0;
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

	m_pEnemyManager->UpdateModelPos();



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

	m_pEnemyManager->Draw();

	//ポーションの描画
	for (auto& object : m_pObjects)
	{
		object->Draw();
	}

	//TODO:UIクラスみたいなのを作ってそこに移動させる
	//装備スロットの描画
	for (int i = 0; i < 4; i++)
	{
		int x = 362 + i * 85;
		int y = 655;
		//DrawBox(x - 30, y - 30, x + 30, y + 30, 0xffffff, false);
		DrawRotaGraph(x,y,0.5f,0.0f,m_slotBgHandle,true);
		if (i == 0)
		{
			DrawRotaGraph(x,y,0.5f,0.0f, m_slotIconHandle[i], true);
		}
	}

	//現在選択しているスロット枠の描画
	DrawBox(362 + m_pPlayer->GetNowSlotNumber() * 85 - 35, 655 - 35, 362 + m_pPlayer->GetNowSlotNumber() * 85 + 35, 655 + 35, 0xff0000, false);

	//右上のUI描画
	DrawRotaGraph(1180, 150, 0.9f, 0.0f, m_slotIconHandle[3], true);
	DrawRotaGraph(1180, 45, 0.75f, 0.0f, m_slotIconHandle[2], true);
	DrawRotaGraph(1180, 40, 0.65f, 0.0f, m_slotIconHandle[1], true);

#ifdef _DEBUG	//デバッグ描画
	//クロスヘアの描画
	auto centerX = Game::kWindowWidth / 2;
	auto centerY = Game::kWindowHeight / 2;
	auto wid = 14;
	auto hei = 2;
	DrawBox(centerX - wid, centerY - hei, centerX + wid, centerY + hei, 0xaaaaaa, true);
	DrawBox(centerX - hei, centerY - wid, centerX + hei, centerY + wid, 0xaaaaaa, true);

	DrawFormatString(640, 0, 0xffffff, "フェーズ番号:%d", m_phaseNum.front());
	DrawFormatString(640, 16, 0xffffff, "次のフェーズまで:%d", m_phaseCount);
#endif
}

/// <summary>
/// オブジェクトを追加する
/// </summary>
void GameManager::AddObject(std::shared_ptr<ObjectBase> pAddObject)
{
	m_pObjects.emplace_back(pAddObject);
}