#include "Player.h"
#include "Input.h"
#include "MyLib.h"
#include "PlayerStateIdle.h"
#include "LoadCSV.h"
#include "HealPortion.h"

#include "Shot.h"
#include "SceneGame.h"

namespace
{
	//デバッグ用のカプセル関係
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;

	/*アナログスティックによる移動関連*/
	constexpr float kMaxSpeed = 0.2f;			//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player():
	CharacterBase(Collidable::Priority::Low, GameObjectTag::Player),
	m_pos(Vec3(0.0f, kCupsuleRadius+ kCupsuleSize,0.0f)),
	temp_moveVec(),
	m_cameraDirection(),
	m_rot(),
	m_cameraAngle(0.0f),
	m_angle(0.0f)
{
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCupsuleRadius;
	sphereCol->m_size = kCupsuleSize;
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init(std::shared_ptr<MyLib::Physics> physics)
{
	m_pPhysics = physics;

	Collidable::Init(physics);

	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->SetNextState(m_pState);
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//プレイヤーの初期位置設定
	rigidbody->Init(true);
	rigidbody->SetPos(m_pos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus("Player");
	//最大HPを設定しておく
	m_hpMax = m_status.hp;
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(SceneGame* pScene)
{
	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->GetNextState()->GetKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->GetNextState();
		m_pState->SetNextState(m_pState);
	}
	
	//ステートの更新
	m_pState->Update();

	if (Input::GetInstance().IsTriggered("B"))
	{
		std::shared_ptr<Shot> shot = std::make_shared<Shot>(GameObjectTag::PlayerShot);
		shot->Init(m_pPhysics);
		shot->Set(m_pos, m_cameraDirection, m_status.atk);

		pScene->AddObject(shot);
	}

	//if (input->GetIsPushedTriggerButton(true) && m_nowSlotIdx == 0 && m_status.hp > 0)
	//{
	//	if (shotTime % kShotIntervalFrame == 0)
	//	{
	//		SoundManager::GetInstance().PlaySE("shot");

	//		auto add = std::make_shared<Shot>();
	//		add->Init(m_pPhysics);
	//		MyLib::Vec3 offset = MyLib::Vec3(0.0f, kModelOffsetY * kModelSizeScale / 2, 0.0f);

	//		auto shotVec = m_cameraDirection;
	//		int offsetX = GetRand(100) - 50;
	//		int offsetY = GetRand(100) - 50;
	//		int offsetZ = GetRand(100) - 50;
	//		MyLib::Vec3 offsetVec = MyLib::Vec3(static_cast<float>(offsetX), static_cast<float>(offsetY), static_cast<float>(offsetZ));
	//		offsetVec = offsetVec.Normalize() * m_shotOffsetPower;
	//		shotVec += offsetVec;

	//		add->Set(m_collisionPos + offset, shotVec, m_status.atk);
	//		m_pShots.emplace_back(add);

	//		m_shotOffsetPower += 0.028f;

	//		if (m_shotOffsetPower >= 0.1f)
	//		{
	//			m_shotOffsetPower = 0.1f;
	//		}
	//	}
	//	shotTime++;
	//}
	//else
	//{
	//	shotTime = 0;
	//}

	//m_difAngle = static_cast<int>(m_shotOffsetPower * 100);
	//if (m_difAngle < 2)
	//{
	//	m_difAngle = 2;
	//}

	//for (auto& shot : m_pShots)
	//{
	//	shot->Update();
	//	if (!shot->GetIsExist())
	//	{
	//		//メモリを解放する
	//		shot->Finalize(m_pPhysics);
	//		shot.reset();
	//		shot = nullptr;
	//	}
	//}

	////不要になった敵をここで削除処理する
	//auto lIt = remove_if(m_pShots.begin(), m_pShots.end(), [](auto& v) {
	//	return v == nullptr;
	//	});
	//m_pShots.erase(lIt, m_pShots.end());
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	//FIX:Drawのなかで座標を変更しているのはどうなの？
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_pos = rigidbody->GetPos();

	//プレイヤー想定のカプセル
	VECTOR low = VGet(m_pos.x, m_pos.y - kCupsuleSize, m_pos.z);
	VECTOR high = VGet(m_pos.x, m_pos.y + kCupsuleSize, m_pos.z);
	DrawCapsule3D(low, high, kCupsuleRadius, kCupsuleDivNum, 0xffffff, 0xffffff, false);

#ifdef _DEBUG	//デバッグ描画
	//入力値の確認
	DrawFormatString(0, 16, 0xff0000, "入力値　: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
	//プレイヤーのステートパターンの確認
	m_pState->DebugDrawState(0,32);
#endif
}

/// <summary>
/// 押し出し処理を行うオブジェクトと衝突したとき
/// </summary>
void Player::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{
//#ifdef _DEBUG
//	std::string message = "プレイヤーが";
//#endif
//	auto tag = colider->GetTag();
//	switch (tag)
//	{
//	case GameObjectTag::Enemy:
//#ifdef _DEBUG
//		message += "敵";
//#endif
//		break;
//	case GameObjectTag::Portion:
//#ifdef _DEBUG
//		message += "ポーション";
//#endif
//		break;
//	}
//#ifdef _DEBUG
//	message += "と当たった！\n";
//	printfDx(message.c_str());
//#endif
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突したとき
/// </summary>
void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
#ifdef _DEBUG
	std::string message = "プレイヤーが";
#endif
	auto tag = colider->GetTag();
	switch (tag)
	{
	case GameObjectTag::Enemy:
#ifdef _DEBUG
		message += "敵";
#endif
		break;
	case GameObjectTag::Portion:
#ifdef _DEBUG
		message += "ポーション";
#endif
		//HPが減っているときのみ回復処理を行う
		//if (m_hpMax > m_status.hp)
		{
			//HPを満タンまで回復させる
			m_status.hp = m_hpMax;

			//ポーションを削除する
			HealPortion* col = dynamic_cast<HealPortion*>(colider.get());
			col->End();

			//回復エフェクトを生成する
			auto pos = rigidbody->GetPos();
			//EffectManager::GetInstance().CreateEffect("Heal", pos);

			//回復SEを流す
			//SoundManager::GetInstance().PlaySE("heal");
		}
		break;
	}
#ifdef _DEBUG
	message += "と当たった！\n";
	printfDx(message.c_str());
#endif
}
