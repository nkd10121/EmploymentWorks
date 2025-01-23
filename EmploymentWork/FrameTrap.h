#pragma once
#include "TrapBase.h"
class FrameTrap : public TrapBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	FrameTrap();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FrameTrap();

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

private:
	Vec3 m_direction;
};

