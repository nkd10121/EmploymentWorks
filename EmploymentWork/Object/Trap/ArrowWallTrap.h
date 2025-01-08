#pragma once
#include "TrapBase.h"
class ArrowWallTrap : public TrapBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ArrowWallTrap();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ArrowWallTrap();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Vec3 pos,Vec3 norm);
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

};

