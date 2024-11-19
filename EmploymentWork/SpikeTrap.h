#pragma once
#include "TrapBase.h"
class SpikeTrap : public TrapBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SpikeTrap();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SpikeTrap();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラス</param>
	void Init(Vec3 pos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

private:
	int m_spikeModel;
};

