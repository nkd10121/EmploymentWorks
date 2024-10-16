#pragma once

//それぞれのシーンでincludeするのめんどいから一旦ここでinclude
//よくないって言われたらそれぞれのシーンにコピペする
#include "SceneManager.h"
#include "Input.h"
#include "DxLib.h"

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
	SceneBase();
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

	virtual void Init() = 0;	// 初期化	リソースのロード完了後に呼ばれる
	virtual void End() = 0;		// シーン終了時に呼ばれる	リソースの解放はここ

	virtual void Update() = 0;	// リソースのロード完了以降呼ばれる	フェード中も呼ばれる
	virtual void Draw() = 0;

#ifdef _DEBUG
	// デバッグ表示(最前面表示)
	virtual void drawDebug() {}
#endif

protected:
	/// <summary>
	/// 現在のシーンを終了させる
	/// </summary>
	void EndThisScene();
public:
	// 継承を行わない処理	SceneManagerから呼び出すのはこっち
	void InitAll();	// シーン個別のinit()の他共通で必要な初期化を行う	
	// これはロード完了後の初回update()で呼ぶのでSceneManager()からは呼ばない
	void EndAll();	// シーン個別のend()の他共通で必要な初期化を行う

	void UpdateAll();	// シーン個別のupdate()の他共通で必要な初期化を行う
	void DrawAll();		// シーン個別のdraw()の他共通で必要な初期化を行う

	bool IsSceneEnd();	// 現在のシーンを終了して次のシーンに遷移する

private:	/*フェード関係*/
	void UpdateFade();
	void DrawFade() const;

	// ロード中表示
	void DrawLoading() const;

	/// <summary>
	/// フェードイン開始
	/// </summary>
	void StartFadeIn();
	/// <summary>
	/// フェードアウト開始
	/// </summary>
	void StartFadeOut();	

private:

	int m_fadeMask;

	// 初期化処理終了判定
	bool m_isInit;

	// フェード関連処理
	int m_fadeBright;	// 0:フェードイン完了	=m_fadeTotalFrame:画面真っ暗
	int m_fadeTotalFrame;
	int m_fadeSpeed;

	// フェード演出
	unsigned int m_fadeColor;
	int m_fadePlayerKind;


	// ロード中表示を行うか
	bool m_isDispLoading;
	// ロードフレーム数カウント
	int m_loadFrame;

	// カーソル表示
	bool m_isDispCursor;

	// シーン遷移
	bool m_isEnd;

	// 処理負荷計測
#ifdef DISP_PROCESS
	LONGLONG	m_updateTime;	// updateにかかった時間(ミリ秒)
	LONGLONG	m_drawTime;		// drawにかかった時間(ミリ秒)
#endif
};

