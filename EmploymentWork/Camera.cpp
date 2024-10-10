#include "Camera.h"
#include "DxLib.h"
#include <cmath>

namespace
{
	/*ÉJÉÅÉâÇÃï`âÊãóó£*/
	constexpr float kCameaNear = 1.0f;	//Near
	constexpr float kCameaFar = 180.0f;	//Far

	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;
}

Camera::Camera():
	m_cameraAngle(-DX_PI_F / 2)
{
	SetCameraNearFar(kCameaNear, kCameaFar);
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Update()
{
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	{
		m_cameraAngle += 0.05f;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT))
	{
		m_cameraAngle -= 0.05f;
	}

	VECTOR cameraPos;
	cameraPos.x = cosf(m_cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(m_cameraAngle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
}