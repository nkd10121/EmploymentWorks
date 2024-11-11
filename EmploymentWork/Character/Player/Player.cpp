#include "Player.h"
#include "Input.h"
#include "MyLib.h"
#include "PlayerStateIdle.h"
#include "LoadCSV.h"
#include "HealPortion.h"

#include "Shot.h"
#include "SceneGame.h"
#include "ModelManager.h"
#include "TrapManager.h"

namespace
{
	/*デバッグ用のカプセル関係*/
	constexpr float kCollisionCapsuleSize = 4.0f;		//カプセルのサイズ
	constexpr float kCollisionCapsuleRadius = 2.0f;		//カプセルの半径
	constexpr int kCollisionCapsuleDivNum = 10;			//カプセルの分け数(?)	//TODO:いらんかったら消す

	/*アナログスティックによる移動関連*/
	constexpr float kMaxSpeed = 0.2f;			//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	/*モデル関係*/
	//キャラクターモデル
	constexpr float kModelScale = 0.065f;		//モデルのサイズ
	//constexpr float kModelScale = 0.05f;		//モデルのサイズ

	//武器モデル
	constexpr float kCrossbowModelScale = 0.64f;	//モデルのサイズ
	const char* kAttachFrameName = "mixamorig:RightHandThumb1";

	/*アニメーション関係*/
	constexpr float kAnimChangeFrame = 10.0f;							//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;		//1フレーム当たりのアニメーション切り替えが進む速さ
	constexpr float kAnimBlendRateMax = 1.0f;							//アニメーションブレンド率の最大


}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	CharacterBase(Collidable::Priority::High, GameObjectTag::Player),
	m_pos(Vec3(0.0f, kCollisionCapsuleRadius + kCollisionCapsuleSize *5, 0.0f)),
	temp_moveVec(),
	m_cameraDirection(),
	m_rot(),
	m_cameraAngle(0.0f),
	m_angle(0.0f),
	m_attackButtonPushCount(0)
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCollisionCapsuleRadius;
	sphereCol->m_size = kCollisionCapsuleSize;

	m_collisionRadius = kCollisionCapsuleRadius;
	m_collisionSize = kCollisionCapsuleSize;

	//キャラクター名を設定
	m_characterName = "Player";
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	//基底クラスで呼んでいるが念のため
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_crossbowHandle);
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

	//プレイヤーモデルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_PLAYER");
	//スケールの変更
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, LoadCSV::GetInstance().GetAnimIdx("Player", "IDLE"));
	m_preAnimIdx = 0;
	m_nowAnimIdx = 0;

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus("Player");
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	//クロスボウのモデルハンドルを取得する
	m_crossbowHandle = ModelManager::GetInstance().GetModelHandle("M_CROSSBOW");
	//スケールの変更
	MV1SetScale(m_crossbowHandle, VGet(kCrossbowModelScale, kCrossbowModelScale, kCrossbowModelScale));


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

	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);

	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendRateMax)
		{
			m_animBlendRate = kAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, kAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	}

	//カメラの座標からプレイヤーを回転させる方向を計算する
	m_angle = -atan2f(m_cameraDirection.z, m_cameraDirection.x) - DX_PI_F / 2;
	m_rot = Vec3(0.0f, m_angle, 0.0f);

	//プレイヤーを回転させる
	MV1SetRotationXYZ(m_modelHandle, m_rot.ToVECTOR());

	if (Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		if (m_attackButtonPushCount % 20 == 0)
		{
			std::shared_ptr<Shot> shot = std::make_shared<Shot>(GameObjectTag::PlayerShot);
			shot->Init(m_pPhysics.lock());
			shot->Set(m_pos, m_cameraDirection, m_status.atk);

			pScene->AddObject(shot);
		}

		m_attackButtonPushCount++;
	}
	else
	{
		m_attackButtonPushCount = 0;
	}

	if (Input::GetInstance().IsTriggered("RB"))
	{

	}
	if (Input::GetInstance().IsTriggered("LB"))
	{

	}

	auto bottomPos = m_pos;
	bottomPos.y -= kCollisionCapsuleSize + kCollisionCapsuleRadius;
	if (m_cameraDirection.y < 0.0f)
	{
		bottomPos.y += 1.0f;
	}
	else if(m_cameraDirection.y > 0.0f)
	{
		bottomPos.y -= 1.0f;
	}

	TrapManager::GetInstance().SelectPoint(bottomPos, m_cameraDirection);
	if (Input::GetInstance().IsTriggered("Y"))
	{
		TrapManager::GetInstance().EstablishTrap(bottomPos, m_cameraDirection,0);
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	//プレイヤーモデルの描画
	MV1DrawModel(m_modelHandle);

	//クロスボウモデルの描画
	MV1DrawModel(m_crossbowHandle);

#ifdef _DEBUG	//デバッグ描画
	//向いてる方向の線
	auto target = m_pos + m_cameraDirection * 1000.0f;
	DrawLine3D(m_pos.ToVECTOR(), target.ToVECTOR(), 0x00ff00);

	//入力値の確認
	DrawFormatString(0, 16, 0xff0000, "入力値　: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
	auto pos = rigidbody->GetPos();
	DrawFormatString(280, 16, 0xff0000, "座標　: %.3f,%.3f,%.3f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 80, 0xff0000, "%d", MV1GetAttachAnim(m_modelHandle, m_currentAnimNo));
	//ステートパターンの確認
	m_pState->DebugDrawState(0, 32);
#endif
}

void Player::UpdateModelPos()
{
	//キャラクターモデル描画座標の更新
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_pos = rigidbody->GetPos();

	auto drawPos = m_pos;
	drawPos.y -= kCollisionCapsuleRadius + kCollisionCapsuleSize;

	MV1SetPosition(m_modelHandle, drawPos.ToVECTOR());

	//クロスボウモデル描画座標の更新
	auto weaponAttachFrameNum = MV1SearchFrame(m_modelHandle, kAttachFrameName);
	auto weaponFrameMat = MV1GetFrameLocalWorldMatrix(m_modelHandle, weaponAttachFrameNum);

	//結果収納用変数
	Vec3 nowLocalPos;
	Vec3 nowLocalRot;
	Vec3 preLocalPos;
	Vec3 preLocalRot;

	LoadCSV::GetInstance().GetCrossbowLocationData(m_nowAnimIdx, nowLocalPos, nowLocalRot);
	LoadCSV::GetInstance().GetCrossbowLocationData(m_preAnimIdx, preLocalPos, preLocalRot);

	auto weaponRot = preLocalRot + (nowLocalRot - preLocalRot) * m_animBlendRate;

	auto xMat = MGetRotX(weaponRot.x);
	auto yMat = MGetRotY(weaponRot.y);
	auto zMat = MGetRotZ(weaponRot.z);

	auto posM = MGetTranslate(nowLocalPos.ToVECTOR());
	posM = MMult(posM, weaponFrameMat);

	auto rotM = MMult(MMult(xMat, yMat), zMat);
	auto setM = MMult(rotM, posM);

	auto scaM = MGetScale(VGet(kCrossbowModelScale, kCrossbowModelScale, kCrossbowModelScale));
	setM = MMult(scaM, setM);

	MV1SetMatrix(m_crossbowHandle, setM);
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
	//	printf(message.c_str());
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
	printf(message.c_str());
#endif
}
