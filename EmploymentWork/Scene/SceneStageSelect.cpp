#include "SceneStageSelect.h"

#include "SceneGame.h"
#include "SceneTitle.h"

#include "MapManager.h"
#include "EffectManager.h"

namespace
{
//#ifdef _DEBUG	//デバッグ描画
	/*テキスト描画関係*/
	constexpr int kTextX = 64;			//テキスト描画X座標
	constexpr int kTextY = 32;			//テキスト描画Y座標
	constexpr int kTextYInterval = 16;	//テキスト描画Y座標の空白
//#endif
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneStageSelect::SceneStageSelect():
	SceneBase("SCENE_STAGESELECT"),
	isNextScene(false),
	m_nowCursor(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneStageSelect::StartLoad()
{
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneStageSelect::IsLoaded() const
{
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneStageSelect::Init()
{
	m_stageNames = LoadCSV::GetInstance().GetAllStageName();

	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load("StageSelect");

	//カメラの初期化
	m_cameraPos = VGet(0.0f, 32.0f, -80.0f);
	m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.ToVECTOR(), m_cameraTarget.ToVECTOR());
}

/// <summary>
/// 終了
/// </summary>
void SceneStageSelect::End()
{
	EffectManager::GetInstance().AllStopEffect();
}

/// <summary>
/// 更新
/// </summary>
void SceneStageSelect::Update()
{
	if (IsLoaded() && m_cameraTarget.y < 20.0f)
	{
		m_cameraTarget.y += 1.0f;
		SetCameraPositionAndTarget_UpVecY(m_cameraPos.ToVECTOR(), m_cameraTarget.ToVECTOR());
	}

	if (isNextScene)
	{
		if (m_nowCursor >= 0)
		{
			auto vec = m_cameraTarget - m_cameraPos;
			vec = vec.Normalize();
			m_cameraPos += vec;
			SetCameraPositionAndTarget_UpVecY(m_cameraPos.ToVECTOR(), m_cameraTarget.ToVECTOR());
		}
		else
		{
			m_nowCursor = 0;
			m_cameraTarget.y -= 4.0f;
			SetCameraPositionAndTarget_UpVecY(m_cameraPos.ToVECTOR(), m_cameraTarget.ToVECTOR());
		}
	}
	// エフェクトの更新
	EffectManager::GetInstance().Update();
}

/// <summary>
/// 描画
/// </summary>
void SceneStageSelect::Draw()
{
	// リソースのロードが終わるまでは描画しないのがよさそう
	// (どちらにしろフェード仕切っているので何も見えないはず)
	if (!IsLoaded())	return;
	if (!IsInitialized())	return;

	// ステージの描画
	MapManager::GetInstance().Draw();

	// エフェクトの描画
	EffectManager::GetInstance().Draw();

#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 0, 0xffffff, "%s", GetNowSceneName());
#endif
	
	DrawString(kTextX - 24, kTextY + kTextYInterval * m_nowCursor, "→", 0xff0000);

	for (int i = 0;i < m_stageNames.size();i++)
	{
		DrawFormatString(kTextX,kTextY + kTextYInterval*i,0xffffff,"%s",m_stageNames[i].c_str());
	}
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneStageSelect::SelectNextSceneUpdate()
{
	//上を入力したら
	if (Input::GetInstance().IsTriggered("UP"))
	{
		//現在選択している項目から一個上にずらす
		m_nowCursor--;

		//もし一番上の項目を選択している状態になっていたら
		if (m_nowCursor < 0)
		{
			//一個下にずらす
			m_nowCursor++;
		}
	}

	//下を入力したら
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		//現在選択している項目から一個下にずらす
		m_nowCursor++;

		//もし一番下の項目を選択している状態になっていたら
		if (m_nowCursor == m_stageNames.size())
		{
			//一個上にずらす
			m_nowCursor--;
		}
	}

	//決定ボタンを押したら現在選択しているシーンに遷移する
	if (Input::GetInstance().IsTriggered("OK"))
	{
		isNextScene = true;
		//ゲームシーンに遷移する
		SceneManager::GetInstance().SetStageIdx(m_nowCursor);
		SceneManager::GetInstance().SetNextScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}

	//Bボタンを押したらセレクトシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		m_nowCursor = -1;
		isNextScene = true;
		SceneManager::GetInstance().SetNextScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}
}