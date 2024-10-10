#pragma once

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

private:
	float m_cameraAngle;	//カメラの向き

};

