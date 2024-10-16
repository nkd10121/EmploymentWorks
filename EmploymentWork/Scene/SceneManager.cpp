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

	//�����̂ݎ��s
	m_pScene.back()->UpdateAll();

	if (m_pScene.back()->IsSceneEnd())
	{
		m_pScene.back()->EndAll();

		m_pScene.back() = m_pNextScene;
		m_pNextScene.reset();
		m_pNextScene = nullptr;

		m_pScene.back()->StartLoad();
	}


	//��{�I�����Ȃ�
	return false;
}

void SceneManager::Draw()
{
	//�擪���珇�ɕ`��
	//�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���
	for (auto& m_pScene : m_pScene)
	{
		m_pScene->DrawAll();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> nextScene)
{
	//���X�g������ۂ����������ւ�����
	if (m_pScene.empty())
	{
		//�����ɒǉ�
		m_pScene.push_back(nextScene);
	}
	else
	{
		m_pNextScene = nextScene;

		//���Ɉ�ȏ゠��Ζ��������ւ���
		//m_pScene.back()->EndAll();
		//m_pScene.back() = nextScene;
		//m_pScene.back()->StartLoad();
	}
}

void SceneManager::ChangeAndClearScene(std::shared_ptr<SceneBase> nextScene)
{
	//�V�[�������Z�b�g
	m_pScene.clear();
	//�����ɒǉ�
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
