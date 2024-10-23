#include "Camera.h"
#include "DxLib.h"
#include "Input.h"
#include <cmath>

namespace
{
	constexpr float kCameraDist = 26.0f;
	constexpr float kCameraHeight = 4.0f;

	constexpr float kCameraNear = 1.0f;
	constexpr float kCameraFar = 360.0f;

	constexpr float kCameraRadius = kCameraNear + 0.2f;

	constexpr float kAngleMoveScaleMax = 3.2f;

	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	// カメラの旋回スピード
	constexpr float kCameraAngleSpeedX = 0.1f;
	constexpr float kCameraAngleSpeedY = 0.05f;

	//カメラの角度制限
	constexpr float kCameraAngleVLimitMin = -DX_PI_F / 2.0f + 0.6f;
	constexpr float kCameraAngleVLimitMax = DX_PI_F / 2.0f - 0.6f;

	// カメラからプレイヤーまでの最大距離
	constexpr float kCameraToPlayerLenghtMax = 175.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera() :
	m_cameraAngleX(0.0f),
	m_cameraAngleY(0.0f),
	m_cameraPos(),
	m_aimPos(),
	m_playerPos(),
	m_angleMoveScale(kAngleMoveScaleMax),
	m_lightHandle(-1)
{
	SetCameraNearFar(kCameraNear, kCameraFar);
	m_lightHandle = CreateDirLightHandle((m_aimPos - m_cameraPos).ConvertToVECTOR());
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
	DeleteLightHandle(m_lightHandle);
}

/// <summary>
/// 初期化
/// </summary>
void Camera::Init()
{
	m_cameraAngleX = 0.0f;
	m_cameraAngleY = 0.0f;
}

/// <summary>
/// 更新
/// </summary>
void Camera::Update()
{
	m_angleMoveScale = 2.0f;

	auto input = Input::GetInstance().GetInputStick(true);

	//入力から角度を計算する
	float inputRateX = input.first / kAnalogInputMax;
	float inputRateY = input.second / kAnalogInputMax;

	if (inputRateX > 0.001f)
	{
		m_cameraAngleX -= m_angleMoveScale * std::abs(inputRateX);
	}
	else if (inputRateX < -0.001f)
	{
		m_cameraAngleX += m_angleMoveScale * std::abs(inputRateX);
	}

	if (inputRateY > 0.001f)
	{
		m_cameraAngleY += m_angleMoveScale * std::abs(inputRateY);
		if (m_cameraAngleY > 90.0f)
		{
			m_cameraAngleY = 89.99f;
		}
	}
	else if (inputRateY < -0.001f)
	{
		m_cameraAngleY -= m_angleMoveScale * std::abs(inputRateY);
		if (m_cameraAngleY < -90.0f)
		{
			m_cameraAngleY = -89.99f;
		}
	}

	// カメラの位置はカメラの水平角度と垂直角度から算出
	// 最初に垂直角度を反映した位置を算出
	MyLib::Vec3 tempPos1;
	float sinParam = sinf(m_cameraAngleY / 180.0f * DX_PI_F);
	float cosParam = cosf(m_cameraAngleY / 180.0f * DX_PI_F);
	tempPos1.x = 0.0f;
	tempPos1.y = sinParam * kCameraDist;
	tempPos1.z = -cosParam * kCameraDist;

	// 次に水平角度を反映した位置を算出
	MyLib::Vec3 tempPos2;
	sinParam = sinf(m_cameraAngleX / 180.0f * DX_PI_F);
	cosParam = cosf(m_cameraAngleX / 180.0f * DX_PI_F);
	tempPos2.x = cosParam * tempPos1.x - sinParam * tempPos1.z;
	tempPos2.y = tempPos1.y;
	tempPos2.z = sinParam * tempPos1.x + cosParam * tempPos1.z;

	m_aimPos = MyLib::Vec3(m_playerPos.x, m_playerPos.y + 9.0f, m_playerPos.z);

	// 算出した座標に注視点の位置を加算したものがカメラの位置になる
	m_cameraPos = tempPos2 + m_aimPos;

	SetLightDirectionHandle(m_lightHandle, (m_aimPos - m_cameraPos).ConvertToVECTOR());

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.ConvertToVECTOR(), m_aimPos.ConvertToVECTOR());
}

/// <summary>
/// カメラが向いている方向ベクトルを取得
/// </summary>
const MyLib::Vec3 Camera::GetDirection() const
{
	return (m_aimPos - m_cameraPos).Normalize();
}
