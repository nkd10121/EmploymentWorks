#pragma once
#include <memory>
#include "MyLib.h"

class CharacterBase;

/// <summary>
/// ステートパターンの基底クラス
/// </summary>
class StateBase : public std::enable_shared_from_this<StateBase>
{
public:
	//ステート
	enum class StateKind
	{
		Null,
		Idle,		//待機
		Walk,		//移動
		Dash,		//ダッシュ
		Jump,		//ジャンプ
		Attack,		//攻撃
		OnHit,		//被ダメージ
		Death,		//死亡
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">所有者</param>
	StateBase(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 現在のStateを返す
	/// </summary>
	/// <returns>現在のState</returns>
	StateKind GetKind() { return m_nowState; }
	/// <summary>
	/// 遷移予定のStateを返す
	/// </summary>
	/// <returns>遷移予定のState</returns>
	const std::shared_ptr<StateBase> GetNextState()const { return m_nextState; }

	/// <summary>
	/// 遷移予定のStateを設定する
	/// </summary>
	/// <param name="state">遷移予定のState</param>
	void SetNextState(std::shared_ptr<StateBase> state) { m_nextState = state; }

	///// <summary>
	///// ダメージを受けた時の処理を行う
	///// </summary>
	///// <param name="collider"></param>
	///// <returns>ダメージ</returns>
	//virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) abstract;



#ifdef _DEBUG
	/// <summary>
	/// 現在のステートのデバッグ描画
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	void DebugDrawState(int x,int y);
#endif

protected:
	/// <summary>
	/// 持ち主がプレイヤーかどうかを判断する
	/// </summary>
	bool CheckPlayer();

	/// <summary>
	/// 持ち主が敵かどうかを判断する
	/// </summary>
	/// <returns></returns>
	bool CheakEnemy();

protected:
	std::weak_ptr<CharacterBase> m_pOwn;	//所有者のポインタ
	std::shared_ptr<StateBase> m_nextState;	//遷移先のStateを保存する

	StateKind m_nowState;					//現在のステート
};

