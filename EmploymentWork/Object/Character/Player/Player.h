#pragma once
#include "Vec3.h"
#include "CharacterBase.h"
#include "StateBase.h"

class SceneGame;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player : public CharacterBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(SceneGame* pScene);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの現在座標を取得する
	/// </summary>
	/// <returns>プレイヤーの現在座標</returns>
	const Vec3 GetPos()const { return rigidbody->GetPos(); }
	/// <summary>
	/// カメラの方向ベクトルを設定する
	/// </summary>
	/// <param name="m_dir">カメラの方向ベクトル</param>
	void SetCameraAngle(Vec3 m_dir) { m_cameraDirection = m_dir; }

	/// <summary>
	/// カメラの方向ベクトルを取得する
	/// </summary>
	/// <returns>カメラの方向ベクトル</returns>
	const Vec3 GetCameraDirecton()const { return m_cameraDirection; }

	/// <summary>
	/// 押し出し処理を行うオブジェクトと衝突したとき
	/// </summary>
	/// <param name="colider">衝突したオブジェクト</param>
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	/// <summary>
	/// 押し出し処理を行わないオブジェクトと衝突したとき
	/// </summary>
	/// <param name="colider">衝突したオブジェクト</param>
	void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

private:
	Vec3 m_pos;				//プレイヤーの描画座標
	Vec3 temp_moveVec;		//入力情報を確認するためのデバッグ用
	Vec3 m_cameraDirection;	//カメラの方向ベクトル
	Vec3 m_rot;				//角度

	float m_cameraAngle;		//カメラの角度
	float m_angle;				//プレイヤーが向いている方向

	std::shared_ptr<StateBase> m_pState;	//Stateパターン
};

