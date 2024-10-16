#include "SceneManager.h"
#include "SceneTitle.h"

SceneManager* SceneManager::m_instance = nullptr;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{

}

bool SceneManager::Update()
{
	Input::GetInstance().Update();

	//末尾のみ実行
	m_pScene.back()->UpdateAll();

	if (m_pScene.back()->IsSceneEnd())
	{
		m_pScene.back()->EndAll();

		m_pScene.back() = m_pNextScene;
		m_pNextScene.reset();
		m_pNextScene = nullptr;

		m_pScene.back()->StartLoad();
	}


	//基本終了しない
	return false;
}

void SceneManager::Draw()
{
	//先頭から順に描画
	//最後に積んだものが最後に描画される
	for (auto& m_pScene : m_pScene)
	{
		m_pScene->DrawAll();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> nextScene)
{
	//リストが空っぽだったら入れ替えずに
	if (m_pScene.empty())
	{
		//末尾に追加
		m_pScene.push_back(nextScene);
	}
	else
	{
		m_pNextScene = nextScene;

		//既に一つ以上あれば末尾を入れ替える
		//m_pScene.back()->EndAll();
		//m_pScene.back() = nextScene;
		//m_pScene.back()->StartLoad();
	}
}

void SceneManager::ChangeAndClearScene(std::shared_ptr<SceneBase> nextScene)
{
	//シーンをリセット
	m_pScene.clear();
	//末尾に追加
	m_pScene.push_back(nextScene);
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_pScene.push_back(scene);
}

void SceneManager::popScene()
{
	m_pScene.pop_back();
}
