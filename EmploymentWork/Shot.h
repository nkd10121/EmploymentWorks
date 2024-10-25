#pragma once
#include "ObjectBase.h"

/// <summary>
/// 遠距離攻撃の弾クラス
/// </summary>
class Shot : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="tag">当たり判定タグ</param>
	Shot(GameObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Shot();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	void Init(std::shared_ptr<MyLib::Physics> physics)override;
	/// <summary>
	/// 当たり判定を削除
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	/// <summary>
	/// 生成座標と向きと攻撃力を設定
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="m_dir">向き</param>
	/// <param name="atk">攻撃力</param>
	void Set(const Vec3& pos, const Vec3& m_dir, const int& atk);
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 終了通知
	/// 当たったオブジェクトから呼ばれる
	/// </summary>
	void End();



	const bool GetIsExist()const { return m_isExist; }
	const int GetAtk()const { return m_atk; }

private:
	Vec3 m_moveDir;

	int m_frameCount;

	float m_angle;
	Vec3 m_rot;

	int m_mapHandle;

	int m_atk;
};