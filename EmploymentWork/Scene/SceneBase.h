#pragma once

//それぞれのシーンでincludeするのめんどいから一旦ここでinclude
//よくないって言われたらそれぞれのシーンにコピペする
#include "SceneManager.h"
#include "Input.h"
#include "DxLib.h"

#include "LoadCSV.h"

#ifdef _DEBUG
// 処理負荷計測
#define DISP_PROCESS
#endif

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneBase(std::string name);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SceneBase() {}

public:	/*派生クラスに継承する必要のある処理*/
	/// <summary>
	/// //リソースのロード開始
	/// </summary>
	virtual void StartLoad() = 0;

	/// <summary>
	/// リソースのロードが終了したかどうか
	/// </summary>
	/// <returns>true : 終了済み, false : 終了していない</returns>
	virtual bool IsLoaded()const = 0;

	/// <summary>
	/// 初期化
	/// MEMO:リソースのロード完了後に呼ばれる
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 終了
	/// MEMO:リソースの解放はこの関数内で行う
	/// </summary>
	virtual void End() = 0;

	/// <summary>
	/// 更新
	/// MEMO:フェード中も呼ばれる
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 次のシーンを選択する更新処理
	/// 基本的にシーン遷移するときはこのシーンを継承する
	/// </summary>
	virtual void SelectNextSceneUpdate() {};

#ifdef _DEBUG
	// デバッグ表示(最前面表示)
	virtual void drawDebug() {}
#endif

protected:
	/// <summary>
	/// 初期化が呼ばれたかどうか
	/// </summary>
	/// <returns>true : 初期化済み, false : 初期化未完了</returns>
	bool IsInitialized() { return m_isInit; }
	
	/// <summary>
	/// 現在のシーンを終了させる
	/// </summary>
	void EndThisScene();

	/// <summary>
	/// リソースの仕分けとロード開始をする
	/// </summary>
	/// <param name="name">読み込みたいリソースデータ配列</param>
	void AssortAndLoadResourse(std::list<LoadCSV::ResourceData> data);

public:	/*継承を行わない処理	SceneManagerから呼び出すのはこっち*/

	/// <summary>
	/// 派生先の初期化とシーン共通で必要な初期化を行う
	/// MEMO:これはロード完了後の初回update()で呼ぶのでSceneManager()からは呼ばない
	/// </summary>
	void InitAll();

	/// <summary>
	/// 派生先の更新とシーン共通で必要な更新を行う
	/// </summary>
	void UpdateAll();

	/// <summary>
	/// 派生先の描画とシーン共通で必要な描画を行う
	/// </summary>
	void DrawAll();

	/// <summary>
	/// 現在のシーンが完全に終了したかどうか
	/// </summary>
	/// <returns>true : 終了した, false : そもそも終了する予定がない or フェード中</returns>
	bool IsSceneEnd();

private:	/*フェード関係*/

	/// <summary>
	/// フェードの更新
	/// </summary>
	void UpdateFade();

	/// <summary>
	/// フェードの描画
	/// </summary>
	void DrawFade() const;

	/// <summary>
	/// ロード中描画
	/// </summary>
	void DrawLoading() const;

	/// <summary>
	/// フェードイン開始
	/// </summary>
	void StartFadeIn();
	/// <summary>
	/// フェードアウト開始
	/// </summary>
	void StartFadeOut();	



protected:
	/// <summary>
	/// フェードインをスキップする
	/// </summary>
	void SkipFadeIn();

	/// <summary>
	/// フェードアウトをスキップする
	/// </summary>
	void SkipFadeOut();

	/// <summary>
	/// 現在のシーンの名前を取得
	/// </summary>
	/// <returns></returns>
	const std::string GetNowSceneName()const { return m_sceneName; }

private:
	bool m_isInit;	//初期化処理終了判定
	bool m_isEnd;	//次のシーンに遷移する

	// フェード関連処理
	int m_fadeAlpha;	//フェードのアルファ値
						//0:フェード中じゃない,0以上:フェード中
	int m_fadeSpeed;	//フェード速度
	unsigned int m_fadeColor;	//フェード時の色

	std::string m_sceneName;

#ifdef DISP_PROCESS
	/*処理負荷計測*/
	LONGLONG	m_updateTime;	// updateにかかった時間(ミリ秒)
	LONGLONG	m_drawTime;		// drawにかかった時間(ミリ秒)
#endif
};

