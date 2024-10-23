#pragma once
#include "CharacterBase.h"

class LoadCSV
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LoadCSV() {};
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~LoadCSV() {};

	//static�ɂ��邱�Ƃ�
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static LoadCSV* m_instance;
public:
	//�R�s�[�R���X�g���N�^������̂̐������ł��Ă��܂�����
	//�R�s�[�R���X�g���N�^���֎~����
	LoadCSV(const LoadCSV&) = delete;
	LoadCSV& operator=(const LoadCSV&) = delete;
	LoadCSV(LoadCSV&&) = delete;
	LoadCSV& operator= (const LoadCSV&&) = delete;

	/// <summary>
	/// LoadCSV��GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static LoadCSV& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new LoadCSV;
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
	/// �X�e�[�^�X�����擾
	/// </summary>
	/// <param name="characterName">�X�e�[�^�X���擾�������L�����N�^�[��</param>
	CharacterBase::Status LoadStatus(const char* characterName);
};