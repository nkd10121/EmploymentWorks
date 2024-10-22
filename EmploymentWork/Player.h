#pragma once
#include "Vec3.h"
#include "CharacterBase.h"

class Player : public CharacterBase
{
public:
	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLib::Physics> physics);
	void Update();
	void Draw();

	const MyLib::Vec3 GetPos()const { return rigidbody.GetPos(); }
	void SetCameraAngle(MyLib::Vec3 m_dir) { m_cameraDirection = m_dir; }

	// 衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

private:
	MyLib::Vec3 m_pos;				//プレイヤーの描画座標
	MyLib::Vec3 temp_moveVec;		//入力情報を確認するためのデバッグ用
	MyLib::Vec3 m_cameraDirection;	//カメラの向き
	MyLib::Vec3 m_rot;				//角度

	float  m_cameraAngle;		//カメラの角度
	float m_angle;				//プレイヤーが向いている方向

};

