#pragma once
#include "StateBase.h"

class Player;

/// <summary>
/// プレイヤーの歩き状態
/// </summary>
class PlayerStateWalk : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own"></param>
	PlayerStateWalk(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	//virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;

	std::string GetWalkAnimName(eDir dir);

private:

private:
	eDir m_dir;
};