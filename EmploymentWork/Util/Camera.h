#pragma once
#include "DxLib.h"
#include "Vec3.h"
#include <memory>

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//カメラが向いている方向ベクトルを取得
	const MyLib::Vec3 GetDirection()const;

	//プレイヤーの座標を設定する
	void SetPlayerPos(MyLib::Vec3 playerPos) { m_playerPos = playerPos; }

private:
	//水平角度
	float m_cameraAngleX;
	//垂直角度
	float m_cameraAngleY;
	//カメラの座標
	MyLib::Vec3 m_cameraPos;
	//注視点
	MyLib::Vec3 m_aimPos;
	//プレイヤーの座標
	MyLib::Vec3 m_playerPos;
	//視点移動速度
	float m_angleMoveScale;
	//ライトハンドル
	int m_lightHandle;
};

