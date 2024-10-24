#pragma once
#include "ObjectBase.h"

class HealPortion : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	HealPortion();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~HealPortion();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラス</param>
	void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// 当たり判定を削除
	/// </summary>
	/// <param name="physics">物理クラス</param>
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 存在しているかどうかを取得
	/// </summary>
	/// <returns></returns>
	const bool GetIsExist()const { return m_isExist; }
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">設定座標</param>
	void SetPosition(Vec3 pos);
	/// <summary>
	/// 終了させる
	/// </summary>
	void End();

private:
	bool m_isExist;	//存在フラグ
	int m_effectCreateFrame;	//フレーム数

	float m_angle;	//角度
	float m_posOffsetY;	//動き幅
};

