#include "SceneManager.h"
#include "SceneTitle.h"

SceneManager* SceneManager::m_instance = nullptr;

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneManager::SceneManager()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneManager::~SceneManager()
{

}

/// <summary>
/// ������
/// </summary>
void SceneManager::Init()
{

}

/// <summary>
/// ���݂̐擪�V�[���̍X�V�֐����Ăяo��
/// </summary>
bool SceneManager::Update()
{
	Input::GetInstance().Update();

	//�����̂ݎ��s
	m_pScene.back()->UpdateAll();

	if (m_pScene.back()->IsSceneEnd())
	{
		for (auto& s : m_pScene)
		{
			s->End();
		}

		//�V�[�������Z�b�g
		m_pScene.clear();
		//�����ɒǉ�
		m_pScene.push_back(m_pNextScene);
		m_pNextScene.reset();
		m_pNextScene = nullptr;

		m_pScene.back()->StartLoad();
	}

	//��{�I�����Ȃ�
	return false;
}

/// <summary>
/// �����Ă���V�[���̕`��֐����Ăяo��
/// </summary>
void SceneManager::Draw()
{
	//�擪���珇�ɕ`��
	//�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���
	for (auto& m_pScene : m_pScene)
	{
		m_pScene->DrawAll();
	}
}

/// <summary>
/// ���s���̃V�[���������Ŏw�肵���V�[���ɐ؂�ւ���
/// </summary>
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

/// <summary>
/// ���ݐ擪�Ŏ��s���̃V�[���̏�ɃV�[�����������
/// </summary>
void SceneManager::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_pScene.push_back(scene);
}

/// <summary>
/// ���݂̐擪�V�[�����폜����
/// </summary>
void SceneManager::PopScene()
{
	m_pScene.pop_back();
}
