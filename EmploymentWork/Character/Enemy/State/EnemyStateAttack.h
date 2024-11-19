#pragma once
#include "StateBase.h"
class EnemyStateAttack : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
	EnemyStateAttack(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;
};

