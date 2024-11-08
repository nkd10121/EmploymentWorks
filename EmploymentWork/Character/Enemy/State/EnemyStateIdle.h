#pragma once
#include "StateBase.h"

/// <summary>
/// 敵の待機状態
/// </summary>
class EnemyStateIdle : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
	EnemyStateIdle(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

private:
	int temp_frame;	//ステートを切り替えるための一時的な変数
};

