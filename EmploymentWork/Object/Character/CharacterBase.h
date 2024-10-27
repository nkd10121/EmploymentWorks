#pragma once
#include "MyLib.h"
#include <map>
#include <memory>
#include <string>

/// <summary>
/// ゲーム内のキャラクターの基底クラス
/// </summary>
class CharacterBase : public MyLib::Collidable
{
public:
	//ステータス構造体
	struct Status
	{
		int hp;			//体力
		int atk;		//攻撃力
		int def;		//防御力
		float speed;	//移動速度
		int point;		//死亡時のポイント
	};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="priority">優先度</param>
	/// <param name="tag">タグ</param>
	CharacterBase(Priority priority, GameObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;


	/// <summary>
	/// 自身の現在のHPを取得
	/// </summary>
	/// <returns>現在のHP</returns>
	const int GetHp()const { return m_status.hp; }
	///// <summary>
	///// 自身の最大HPを取得
	///// </summary>
	///// <returns>最大HP</returns>
	//const int GetMaxHp()const { return m_maxHp; }
	/// <summary>
	/// 物理データを取得
	/// </summary>
	/// <returns>物理データ</returns>
	std::shared_ptr<MyLib::Rigidbody> GetRigidbody(){ return rigidbody; }
	/// <summary>
	/// アニメーションが終了したかどうかを取得
	/// </summary>
	/// <returns></returns>
	const bool GetIsAnimationEnd()const { return m_isAnimationFinish; }

protected:
	/// <summary>
	/// アニメーションの更新
	/// </summary>
	/// <param name="attachNo">進行させたいアニメーション番号</param>
	/// <returns>ループしたかどうか</returns>
	bool UpdateAnim(int attachNo, float startTime = 0.0f);
	/// <summary>
	///	アニメーションの変更
	/// </summary>
	/// <param name="animIndex">変更後のアニメーション番号</param>
	void ChangeAnim(int animIndex, float animSpeed = 0.5f);


protected:
	int m_modelHandle;	//モデルハンドル
	
	Status m_status;	//ステータス
	int m_hpMax;

	std::shared_ptr<MyLib::Physics> m_pPhysics;	

	/*アニメーション関係*/
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;
	int m_equipAnimNo;			//1つ前ののアニメーション
	int m_currentAnimNo;		//現在のアニメーション
	int m_prevAnimNo;			//変更前のアニメーション
	float m_animBlendRate;		//アニメーションの合成割合
	//0.0f:prevが再生
	//1.0f:currntが再生

	float m_animSpeed;			//アニメーション再生速度
	bool m_isAnimationFinish;	//アニメーションが終わったかどうか
};

