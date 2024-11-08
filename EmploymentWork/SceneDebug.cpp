#include "SceneDebug.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "ScenePause.h"

#include "SoundManager.h"
#include "LoadCSV.h"

namespace
{
	const std::string kSceneName[] =
	{
		"SceneTitle",
		"SceneGame",
		"ScenePause",
	};

	constexpr int kTextX = 64;
	constexpr int kTextY = 32;
	constexpr int kTextYInterval = 16;
}

SceneDebug::SceneDebug() :
	SceneBase("SCENE_DEBUG")
{
}

SceneDebug::~SceneDebug()
{
}

void SceneDebug::StartLoad()
{
	//このシーンでロードするべきリソースのパスを取得
	auto loadResourceData = LoadCSV::GetInstance().GetLoadResourcePath(GetNowSceneName());

	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	//リソースデータ群をみてリソースのロードを開始する
	AssortAndLoadResourse(loadResourceData);

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneDebug::IsLoaded() const
{
	return true;
}

void SceneDebug::Init()
{
	m_destinationScene = static_cast<eDestination>(static_cast<int>(eDestination::Start) + 1);
}

void SceneDebug::End()
{
}

void SceneDebug::Update()
{
	if (Input::GetInstance().IsTriggered("UP"))
	{
		m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) - 1);

		if (m_destinationScene == eDestination::Start)
		{
			m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) + 1);
		}
	}

	if (Input::GetInstance().IsTriggered("DOWN"))
	{
		m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) + 1);

		if (m_destinationScene == eDestination::Last)
		{
			m_destinationScene = static_cast<eDestination>(static_cast<int>(m_destinationScene) - 1);
		}
	}

	if (Input::GetInstance().IsTriggered("OK"))
	{
		if (m_destinationScene == eDestination::Title)
		{
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::InGame)
		{
			SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
			EndThisScene();
			return;
		}
		else if (m_destinationScene == eDestination::Pause)
		{
			SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
			return;
		}
	}
}

void SceneDebug::Draw()
{
	DrawString(kTextX - 24, kTextY + kTextYInterval * (m_destinationScene - 1), "→", 0xff0000);

	int y = kTextY;
	for (auto& name : kSceneName)
	{
		DrawString(kTextX,y,name.c_str(),0xffffff);

		y += kTextYInterval;
	}

#ifdef _DEBUG
	DrawString(0, 0, "DEBUG", 0xffffff);
#endif
}
