#pragma once
#include "StateBase.h"

/// <summary>
/// プレイヤーのダッシュ状態
/// </summary>
class PlayerStateDash : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own"></param>
	PlayerStateDash(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;
};

