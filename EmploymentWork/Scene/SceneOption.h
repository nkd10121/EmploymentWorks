#pragma once
#include "SceneBase.h"

/// <summary>
/// オプションシーン
/// </summary>
class SceneOption : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneOption();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneOption();

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

	/// <summary>
	/// 次のシーンを選択する更新処理
	/// </summary>
	virtual void SelectNextSceneUpdate()override;

private:
	//更新メンバ関数ポインタ
	void (SceneOption::* m_updateFunc)();
	//更新(BGM設定)
	void BgmUpdate();
	//更新(SE設定)
	void SeUpdate();
	//更新(感度設定)
	void SensitivityUpdate();
	//更新(フルスクリーン設定)
	void FullScreenUpdate();
};

