#pragma once
#include "CharacterBase.h"
#include "StateBase.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	//種類
	enum EnemyKind
	{
		EnemyNormal,
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBase();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBase();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Init(std::shared_ptr<MyLib::Physics> physics) {};
	/// <summary>
	/// 終了
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	virtual void Finalize() {};
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {};
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() {};

	void SetPos(Vec3 pos);

	/// <summary>
	/// 終了(他から呼ぶ)
	/// </summary>
	void End() { m_isExist = false; }

	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider);
	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnTriggerEnter(const std::shared_ptr<Collidable>& colider)override;

	/// <summary>
	/// 存在フラグを取得
	/// </summary>
	/// <returns>存在フラグ</returns>
	const bool GetIsExist()const { return m_isExist; }

protected:
	EnemyKind kind;		//種類
	bool m_isExist;		//存在フラグ

	int m_modelHandle;	//モデルハンドル

	Vec3 m_drawPos;		//描画座標

	GameObjectTag m_hitObjectTag;	//あったオブジェクトタグ

	std::shared_ptr<StateBase> m_pState;	//Stateパターン

};