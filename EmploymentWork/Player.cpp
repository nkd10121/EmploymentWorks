#include "Player.h"
#include "Input.h"
#include "MyLib.h"
#include "PlayerStateIdle.h"

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

Player::Player():
	CharacterBase(Collidable::Priority::Low, GameObjectTag::Player),
	m_pos(MyLib::Vec3(0.0f, kCupsuleRadius+ kCupsuleSize,0.0f)),
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

Player::~Player()
{
}

void Player::Init(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Init(physics);

	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->SetNextState(m_pState);
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//プレイヤーの初期位置設定
	rigidbody.Init(false);
	rigidbody.SetPos(m_pos);
	rigidbody.SetNextPos(rigidbody.GetPos());
}

void Player::Update()
{
	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->GetNextState()->GetKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->GetNextState();
		m_pState->SetNextState(m_pState);
	}

	//コントローラーの左スティックの入力を取得
	auto input = Input::GetInstance().GetInputStick(false);
	temp_moveVec = MyLib::Vec3(input.first,0.0f,-input.second);
	//移動方向を設定する
	auto temp_moveVec = MyLib::Vec3(input.first, 0.0f, -input.second);
	//移動ベクトルの長さを取得する
	float len = temp_moveVec.Size();

	//ベクトルの長さを0.0～1.0の割合に変換する
	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する(デッドゾーン)
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	temp_moveVec = temp_moveVec.Normalize();
	float speed = /*m_status.speed*/1.0f * rate;

	temp_moveVec = temp_moveVec * speed;

	//プレイヤーの正面方向を計算して正面方向を基準に移動する
//カメラの正面方向ベクトル
	MyLib::Vec3 front(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//向きベクトル*移動量
	front = front * temp_moveVec.z;
	//カメラの右方向ベクトル
	MyLib::Vec3 right(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);
	//向きベクトル*移動量
	right = right * (-temp_moveVec.x);

	//移動ベクトルの生成
	temp_moveVec = front + right;
	temp_moveVec = temp_moveVec.Normalize() * speed;
	//移動処理
	//MV1SetPosition(m_modelHandle, m_collisionPos.ConvertToVECTOR());

	//カメラの座標からプレイヤーを回転させる方向を計算する
	m_angle = -atan2f(m_cameraDirection.z, m_cameraDirection.x) - DX_PI_F / 2;
	m_rot = MyLib::Vec3(0.0f, m_angle, 0.0f);
	
	MyLib::Vec3 prevVelocity = rigidbody.GetVelocity();
	MyLib::Vec3 newVelocity = MyLib::Vec3(temp_moveVec.x, prevVelocity.y, temp_moveVec.z);
	rigidbody.SetVelocity(newVelocity);

	m_pState->Update();
}

void Player::Draw()
{
	//FIX:Drawのなかで座標を変更しているのはどうなの？
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_pos = rigidbody.GetPos();

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

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& colider)
{
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& colider)
{
}
