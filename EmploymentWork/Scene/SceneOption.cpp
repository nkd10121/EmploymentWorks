#include "SceneOption.h"

#include "Setting.h"
#include "FontManager.h"

#include <sstream>
#include <iomanip> 

namespace
{
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneOption::SceneOption():
	SceneBase("SCENE_OPTION")
{
	m_updateFunc = &SceneOption::BgmUpdate;
}

/// <summary>
/// デストラクタ
/// </summary>
SceneOption::~SceneOption()
{
}

/// <summary>
/// //リソースのロード開始
/// </summary>
void SceneOption::StartLoad()
{
}

/// <summary>
/// リソースのロードが終了したかどうか
/// </summary>
bool SceneOption::IsLoaded() const
{
	return true;
}

/// <summary>
/// 初期化
/// </summary>
void SceneOption::Init()
{
	//フェードアウトをスキップする
	//SkipFadeOut();
}

/// <summary>
/// 終了
/// </summary>
void SceneOption::End()
{
}

/// <summary>
/// 更新
/// </summary>
void SceneOption::Update()
{
	//状態の更新
	(this->*m_updateFunc)();
}

/// <summary>
/// 描画
/// </summary>
void SceneOption::Draw()
{
#ifdef _DEBUG	//デバッグ描画
	DrawFormatString(0, 32, 0xffffff, "%f", GetNowSceneName());
#endif
	std::stringstream ss;
	ss << "BGM:" << std::fixed << std::setprecision(2) << Setting::GetInstance().GetBGMVolume();
	auto text = ss.str();
	FontManager::GetInstance().DrawCenteredText(200, 200, text, 0xffffff, 32, 0x000000);

	ss.str(""); // ここで初期化
	ss << "SE:" << std::fixed << std::setprecision(2) << Setting::GetInstance().GetSEVolume();
	text = ss.str();
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100, text, 0xffffff, 32, 0x000000);

	ss.str(""); // ここで初期化
	ss << "Sensitivity:" << std::fixed << std::setprecision(2) << Setting::GetInstance().GetSensitivity();
	text = ss.str();
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 2, text, 0xffffff, 32, 0x000000);

	ss.str(""); // ここで初期化
	text = "FullScreen:";
	if (Setting::GetInstance().GetIsFullScreen())
	{
		text += "●";
	}
	else
	{
		text += "〇";
	}
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 3, text, 0xffffff, 32, 0x000000);
}

/// <summary>
/// 次のシーンを選択する更新処理
/// </summary>
void SceneOption::SelectNextSceneUpdate()
{
	//Bボタンを押したら元のシーンに戻る
	if (Input::GetInstance().IsTriggered("CANCEL"))
	{
		EndThisScene();
		return;
	}
}

void SceneOption::BgmUpdate()
{
	if (Input::GetInstance().IsTriggered("RIGHT"))
	{
		Setting::GetInstance().SetBGMVolume(min(Setting::GetInstance().GetBGMVolume() + 0.1f, 1.0f));
	}
	if (Input::GetInstance().IsTriggered("LEFT"))
	{
		Setting::GetInstance().SetBGMVolume(max(Setting::GetInstance().GetBGMVolume() - 0.1f, 0.0f));
	}

	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::SeUpdate;
	}
}

void SceneOption::SeUpdate()
{
	if (Input::GetInstance().IsTriggered("RIGHT"))
	{
		Setting::GetInstance().SetSEVolume(min(Setting::GetInstance().GetSEVolume() + 0.1f, 1.0f));
	}
	if (Input::GetInstance().IsTriggered("LEFT"))
	{
		Setting::GetInstance().SetSEVolume(max(Setting::GetInstance().GetSEVolume() - 0.1f, 0.0f));
	}

	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_updateFunc = &SceneOption::BgmUpdate;
	}
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::SensitivityUpdate;
	}
}

void SceneOption::SensitivityUpdate()
{
	if (Input::GetInstance().IsTriggered("RIGHT"))
	{
		Setting::GetInstance().SetSensitivity(min(Setting::GetInstance().GetSensitivity() + 0.1f, 1.0f));
	}
	if (Input::GetInstance().IsTriggered("LEFT"))
	{
		Setting::GetInstance().SetSensitivity(max(Setting::GetInstance().GetSensitivity() - 0.1f, 0.0f));
	}

	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_updateFunc = &SceneOption::SeUpdate;
	}
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::FullScreenUpdate;
	}
}

void SceneOption::FullScreenUpdate()
{
	if (Input::GetInstance().IsTriggered("OK"))
	{
		Setting::GetInstance().SetIsFullScreen(!Setting::GetInstance().GetIsFullScreen());
		ChangeWindowMode(!Setting::GetInstance().GetIsFullScreen());
	}

	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_updateFunc = &SceneOption::SensitivityUpdate;
	}
}
