#pragma once
#include "TrapBase.h"
class FlameTrap : public TrapBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FlameTrap();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FlameTrap();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Vec3 pos, Vec3 direction)override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	void SetRot(Vec3 vec)override;

private:
	Vec3 m_direction;
};

