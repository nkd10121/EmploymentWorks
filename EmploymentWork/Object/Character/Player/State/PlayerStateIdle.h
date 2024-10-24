#pragma once
#include "StateBase.h"

/// <summary>
/// プレイヤーの待機状態
/// </summary>
class PlayerStateIdle : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
	PlayerStateIdle(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;
	///// <summary>
	///// 被ダメージ
	///// </summary>
	///// <param name="collider">当たったオブジェクト</param>
	///// <returns>ダメージ数?</returns>
	//virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;
};