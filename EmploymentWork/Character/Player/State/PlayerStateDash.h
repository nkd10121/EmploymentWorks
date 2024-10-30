#pragma once
#include "StateBase.h"

/// <summary>
/// プレイヤーのダッシュ状態
/// </summary>
class PlayerStateDash : public StateBase
{
private:
	const enum eDir : int
	{
		Right,
		BackRight,
		Back,
		BackLeft,
		Left,
		ForwardLeft,
		Forward,
		ForwardRight
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
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

