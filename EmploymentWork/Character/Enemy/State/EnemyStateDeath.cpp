#include "EnemyStateDeath.h"
#include "CharacterBase.h"

#include "LoadCSV.h"
EnemyStateDeath::EnemyStateDeath(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Death;

	//アニメーションを変える
	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "DEATH_A"));
}

void EnemyStateDeath::Init()
{
}

void EnemyStateDeath::Update()
{
}
