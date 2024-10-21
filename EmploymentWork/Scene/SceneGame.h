#pragma once
#include "SceneBase.h"
//テスト用
#include <list>

//プロトタイプ宣言
class Player;	//プレイヤー

/// <summary>
/// ゲームシーン
/// </summary>
class SceneGame : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneGame();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneGame();

	/// <summary>
	/// //リソースのロード開始
	/// </summary>
	virtual void StartLoad() override;

	/// <summary>
	/// リソースのロードが終了したかどうか
	/// </summary>
	/// <returns>true : 終了済み, false : 終了していない</returns>
	virtual bool IsLoaded() const override;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void End() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

private:
	std::list<int> temp_handle;	//テスト用　TODO:いらなくなったら消せ

	std::shared_ptr<Player> m_pPlayer;
};

