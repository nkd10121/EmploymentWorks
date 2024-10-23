#pragma once
#include <string>
#include <list>



/// <summary>
/// 3D���f���̃n���h�����Ǘ�����N���X
/// </summary>
class ModelManager
{
public:
	/// <summary>
	/// �n���h��
	/// </summary>
	struct Handle
	{
		std::string path;	//���f���p�X
		int handle;			//���f���n���h��
		bool isEternal;		//�풓�t���O
	};

private:
	//�R���X�g���N�^
	ModelManager() {};
	//�f�X�g���N�^
	virtual ~ModelManager();
	//static�ɂ��邱�Ƃ�
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static ModelManager* m_instance;

public:
	//�R�s�[�R���X�g���N�^������̂̐������ł��Ă��܂�����
	//�R�s�[�R���X�g���N�^���֎~����
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
	ModelManager(ModelManager&&) = delete;
	ModelManager& operator= (const ModelManager&&) = delete;

	/// <summary>
	/// ModelManager��GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static ModelManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new ModelManager;
		}

		return *m_instance;
	}

	//��������Y���ƕ��ʂɃ��������[�N
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	/// <summary>
	/// �w�肵���p�X�����f�������[�h����
	/// </summary>
	/// <param name="path">���f���p�X</param>
	/// <param name="isEternal">�풓�t���O</param>
	void LoadModel(std::string path, bool isEternal = false);

	/// <summary>
	/// ���f���n���h�����擾����
	/// </summary>
	/// <param name="path">���f���p�X</param>
	/// <returns>-1 : �G���[, -1�ȊO : ���f���̕����n���h��</returns>
	int GetModelHandle(std::string path);

	/// <summary>
	/// �풓�t���O��false�̃n���h����S�폜����
	/// </summary>
	void Clear();

	/// <summary>
	/// ���[�h����Ă��邷�ׂẴ��f���̕����O�n���h����Ԃ�
	/// </summary>
	/// <returns></returns>
	const std::list<int> GetPrimitiveModelHandles()const;

private:
	//���f���n���h��
	std::list<Handle> m_handles;

};

