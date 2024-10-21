#include "Player.h"
#include "Input.h"

namespace
{
	//デバッグ用のカプセル関係
	constexpr float kCupsuleSize = 2.0f;
	constexpr float kCupsuleRadius = 2.0f;
	constexpr int kCupsuleDivNum = 10;
}

Player::Player():
	m_pos(),
	temp_moveVec()
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	//コントローラーの左スティックの入力を取得
	auto input = Input::GetInstance().GetInputStick(false);
	temp_moveVec = MyLib::Vec3(input.first,0.0f,input.second);
	temp_moveVec = temp_moveVec.Normalize();
	//移動ベクトルの長さを取得する
	float len = temp_moveVec.Size();
	
}

void Player::Draw()
{
	//プレイヤー想定のカプセル
	VECTOR low = VGet(m_pos.x, m_pos.y - kCupsuleSize, m_pos.z);
	VECTOR high = VGet(m_pos.x, m_pos.y + kCupsuleSize, m_pos.z);
	DrawCapsule3D(low, high, kCupsuleRadius, kCupsuleDivNum, 0xffffff, 0xffffff, false);


#ifdef _DEBUG	//デバッグ描画
	//入力値の確認
	DrawFormatString(0, 16, 0xff0000, "入力値　: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
#endif
}
