#pragma once
#include "EnemyBase.h"

/// <summary>
/// 基本的な敵クラス
/// </summary>
class EnemyNormal : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyNormal();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyNormal();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// 終了
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Finalize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 3Dモデルの座標更新
	/// </summary>
	virtual void UpdateModelPos()override;
};

