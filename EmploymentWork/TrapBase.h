#pragma once
#include "ObjectBase.h"

class TrapBase : public ObjectBase
{
	public:
	//ステータス構造体
	struct Status
	{
		int atk;
		float searchRange;
		float atkRange;
		int coolTime;
		int cost;
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TrapBase(GameObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TrapBase();

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// モデルのロードを申請する
	/// </summary>
	void LoadModel()override;

	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">設定座標</param>
	void SetPosition(Vec3 pos)override;

	/// <summary>
	/// 終了させる
	/// </summary>
	void End();

protected:
	Status status;
	int m_attack;
	int m_isExist;	//存在フラグ

};

