#include "SceneOption.h"

#include "Setting.h"
#include "SoundManager.h"
#include "FontManager.h"

#include <sstream>
#include <iomanip> 

namespace
{
	constexpr int kKeyRepeatInitFrame = 30;
}

/// <summary>
/// コンストラクタ
/// </summary>
SceneOption::SceneOption():
	SceneBase("SCENE_OPTION"),
	m_pushCount(0),
	m_keyRepeatFrame(kKeyRepeatInitFrame)
{
	m_updateFunc = &SceneOption::MasterUpdate;
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
	if (!Input::GetInstance().IsPushed("RIGHT") && !Input::GetInstance().IsPushed("LEFT"))
	{
		m_pushCount = 0;
		m_keyRepeatFrame = kKeyRepeatInitFrame;
	}

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
	std::string text = "Master:";
	text += std::to_string(static_cast<int>(Setting::GetInstance().GetMasterVolume() * 100));
	FontManager::GetInstance().DrawCenteredText(200, 200, text, 0xffffff, 32, 0x000000);

	text = "BGM:";
	text += std::to_string(static_cast<int>(Setting::GetInstance().GetBGMVolume() * 100));
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100, text, 0xffffff, 32, 0x000000);

	text = "SE:";
	text += std::to_string(static_cast<int>(Setting::GetInstance().GetSEVolume() * 100));
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 2, text, 0xffffff, 32, 0x000000);

	text = "Sensitivity:";
	text += std::to_string(static_cast<int>(Setting::GetInstance().GetSensitivity() * 100));
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 3, text, 0xffffff, 32, 0x000000);

	text = "FullScreen:";
	if (Setting::GetInstance().GetIsFullScreen())
	{
		text += "●";
	}
	else
	{
		text += "〇";
	}
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 4, text, 0xffffff, 32, 0x000000);

	text = "操作説明を描画:";
	if (Setting::GetInstance().GetIsDrawOperation())
	{
		text += "●";
	}
	else
	{
		text += "〇";
	}
	FontManager::GetInstance().DrawCenteredText(200, 200 + 100 * 5, text, 0xffffff, 32, 0x000000);
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

void SceneOption::MasterUpdate()
{
	if (Input::GetInstance().IsPushed("RIGHT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetMasterVolume(min(Setting::GetInstance().GetMasterVolume() + 0.01f, 1.0f));
			SoundManager::GetInstance().BGMChangeVolume();
		}
		m_pushCount++;
	}
	if (Input::GetInstance().IsPushed("LEFT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetMasterVolume(max(Setting::GetInstance().GetMasterVolume() - 0.01f, 0.0f));
			SoundManager::GetInstance().BGMChangeVolume();
		}
		m_pushCount++;
	}

	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::BgmUpdate;
	}
}

void SceneOption::BgmUpdate()
{
	if (Input::GetInstance().IsPushed("RIGHT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetBGMVolume(min(Setting::GetInstance().GetBGMVolume() + 0.01f, 1.0f));
			SoundManager::GetInstance().BGMChangeVolume();
		}
		m_pushCount++;
	}
	if (Input::GetInstance().IsPushed("LEFT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetBGMVolume(max(Setting::GetInstance().GetBGMVolume() - 0.01f, 0.0f));
			SoundManager::GetInstance().BGMChangeVolume();
		}
		m_pushCount++;
	}

	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_updateFunc = &SceneOption::MasterUpdate;
	}
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::SeUpdate;
	}
}

void SceneOption::SeUpdate()
{
	if (Input::GetInstance().IsPushed("RIGHT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetSEVolume(min(Setting::GetInstance().GetSEVolume() + 0.01f, 1.0f));
		}
		m_pushCount++;
	}
	if (Input::GetInstance().IsPushed("LEFT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetSEVolume(max(Setting::GetInstance().GetSEVolume() - 0.01f, 0.0f));
		}
		m_pushCount++;
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
	if (Input::GetInstance().IsPushed("RIGHT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetSensitivity(min(Setting::GetInstance().GetSensitivity() + 0.01f, 1.0f));
		}
		m_pushCount++;
	}
	if (Input::GetInstance().IsPushed("LEFT"))
	{
		if (m_pushCount == 0 || m_pushCount > m_keyRepeatFrame)
		{
			if (m_pushCount > m_keyRepeatFrame)
			{
				m_pushCount = 0;
				m_keyRepeatFrame = max(m_keyRepeatFrame * 0.6f, 0);
			}
			Setting::GetInstance().SetSensitivity(max(Setting::GetInstance().GetSensitivity() - 0.01f, 0.0f));
		}
		m_pushCount++;
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
	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_updateFunc = &SceneOption::DrawOperationUpdate;
	}
}

void SceneOption::DrawOperationUpdate()
{
	if (Input::GetInstance().IsTriggered("OK"))
	{
		Setting::GetInstance().SetIsDrawOperation(!Setting::GetInstance().GetIsDrawOperation());
	}

	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_updateFunc = &SceneOption::FullScreenUpdate;
	}
}
