#pragma once
#include <list>
#include <memory>

class SceneBase;

/// <summary>
/// �V�[�����Ǘ�����N���X
/// </summary>
class SceneManager
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~SceneManager();

	static SceneManager* m_instance;	//�C���X�^���X
public:
	//�R�s�[�R���X�g���N�^���֎~����
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator= (const SceneManager&&) = delete;

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static SceneManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new SceneManager;
		}
		return *m_instance;
	}

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �����Ă���V�[���̍X�V�֐����Ăяo��
	/// </summary>
	/// <returns>true : �Q�[���I��, false : �Q�[�����s</returns>
	bool Update();
	/// <summary>
	/// �����Ă���V�[���̕`��֐����Ăяo��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���s���̃V�[���������Ŏw�肵���V�[���ɐ؂�ւ���
	/// </summary>
	/// <param name="nextScene">�J�ڐ�̃V�[��</param>
	void ChangeScene(std::shared_ptr<SceneBase> nextScene);
	/// <summary>
	/// ���ݐ擪�Ŏ��s���̃V�[���̏�ɃV�[�����������
	/// </summary>
	/// <param name="scene">�J�ڐ�̃V�[��</param>
	void PushScene(std::shared_ptr<SceneBase> scene);
	/// <summary>
	/// ���݂̐擪�V�[�����폜����
	/// </summary>
	void popScene();
private:
	std::list<std::shared_ptr<SceneBase>> m_pScene;		//���݂̃V�[��
	std::shared_ptr<SceneBase> m_pNextScene;			//�J�ڗ\��̃V�[��
};

