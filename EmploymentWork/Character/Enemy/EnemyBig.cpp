#include "EnemyBig.h"
#include "ResourceManager.h"

namespace
{
	//カプセルの大きさ
	constexpr float kCollisionCapsuleSize = 5.0f;
	//カプセルの半径
	constexpr float kCollisionCapsuleRadius = 3.0f;
	//カプセルのY座標のオフセット
	constexpr float kPosOffsetY = 2.0f;	
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBig::EnemyBig() :
	EnemyBase()
{
	//キャラクター名を設定
	m_characterName = "EnemyBig";
}

/// <summary>
/// 初期化
/// </summary>
void EnemyBig::Init()
{
	//モデルが大きめのため、ルートを少し上にずらす
	for (auto& p : m_route)
	{
		p.pos.y += kPosOffsetY;
	}

	//モデルハンドルを取得
	m_modelHandle = ResourceManager::GetInstance().GetHandle("M_ENEMY3");

	//当たり判定のサイズを設定
	m_collisionRadius = kCollisionCapsuleRadius;
	m_collisionSize = kCollisionCapsuleSize;

	EnemyBase::Init();
}