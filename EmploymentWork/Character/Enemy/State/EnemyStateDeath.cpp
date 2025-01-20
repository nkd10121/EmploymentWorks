#include "EnemyStateDeath.h"
#include "CharacterBase.h"
#include "EnemyBase.h"

#include "LoadCSV.h"
#include "DrawUI.h"
#include "FontManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
EnemyStateDeath::EnemyStateDeath(std::shared_ptr<CharacterBase> own):
	StateBase(own)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Death;

	//アニメーションを変える
	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "DEATH_A"));

	m_frame = 0;
}

/// <summary>
/// 初期化
/// </summary>
void EnemyStateDeath::Init(std::string id)
{
}

/// <summary>
/// 更新
/// </summary>
void EnemyStateDeath::Update()
{
	//持ち主が敵かどうかをチェックする
	if (!CheakEnemy())	return;

	auto own = std::dynamic_pointer_cast<EnemyBase>(m_pOwn.lock());

	auto pos = own->GetPos();
	auto drawPos = ConvWorldPosToScreenPos(pos.ToVECTOR());

	auto point = own->GetDropPoint();

	//ドロップする罠ポイントの描画
	DrawUI::GetInstance().RegisterDrawRequest([=]()
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 6 * m_frame);
		FontManager::GetInstance().DrawCenteredText(static_cast<int>(drawPos.x), static_cast<int>(drawPos.y) - 80 - m_frame / 4, std::to_string(point), 0x9effff, 32);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}, 2);

	m_frame++;
}