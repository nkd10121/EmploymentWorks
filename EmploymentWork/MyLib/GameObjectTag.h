#pragma once

// 当たったものの判別を行うためのタグ
enum class GameObjectTag
{
	Player,			// プレイヤー
	Enemy,			// 敵
	Portion,		//回復ポーション
	PlayerShot,		//プレイヤーの遠距離攻撃の弾
	EnemyShot,		//敵の遠距離攻撃の弾
};