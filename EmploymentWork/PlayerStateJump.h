#pragma once
#include "StateBase.h"

/// <summary>
/// プレイヤーのジャンプ状態
/// </summary>
class PlayerStateJump : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
	PlayerStateJump(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

private:
	int m_jumpCount;
};

