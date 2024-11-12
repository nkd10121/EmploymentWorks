#include "PlayerStateDeath.h"
#include "Player.h"

#include "LoadCSV.h"

PlayerStateDeath::PlayerStateDeath(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Death;

	//アニメーションを変える
	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx("Player", "DEATH_A"));
}

void PlayerStateDeath::Init()
{
}

void PlayerStateDeath::Update()
{
}
