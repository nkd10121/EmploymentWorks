#pragma once
#include "DxLib.h"
#include<unordered_map>
#include<map>
#include<string>

enum class InputType
{
	keyboard,	//�L�[�{�[�h
	pad			//�p�b�h
};

//�G�C���A�X�錾
using InputTable_t = std::unordered_map<std::string, int>;

/// <summary>
/// ���͏��Ǘ��N���X
/// </summary>
class Input
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Input();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Input() {};

	static Input* m_instance;	//�C���X�^���X

public:
	//�R�s�[�R���X�g���N�^���֎~����
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator= (const Input&&) = delete;

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Input& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Input;
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
	/// ���͏����X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �w��̃R�}���h�������ꂽ�u�ԂȂ̂�
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns> true : �����ꂽ�u��, false : ������Ă��Ȃ����A��������</returns>
	bool IsTriggered(const char* command)const;

	/// <summary>
	/// �w��̃R�}���h�������������Ă��邩�ǂ���
	/// </summary>
	/// <param name="command">�R�}���h������</param>
	/// <returns>true : �����������Ă���, false : ������Ă��Ȃ����A�����ꂽ�u��</returns>
	bool IsPushed(const char* command)const;

	/// <summary>
	/// �X�e�B�b�N�̓��͏����擾
	/// </summary>
	/// <param name="isRight">�E�X�e�B�b�N���ǂ����@false : ���X�e�B�b�N, true : �E�X�e�B�b�N</param>
	/// <returns></returns>
	std::pair<float, float> GetInputStick(bool isRight)const;

private:
	InputTable_t m_commandTable;	//�R�}���h�z��

	//�R�}���h�̓��͂��o���Ă������߂̔z��
	std::map < std::string, bool> m_inputData;		//���݂̓���
	std::map < std::string, bool> m_lastInputData;	//���O�̓���

	//�R���g���[���[�̓��͏��
	DINPUT_JOYSTATE m_padState;

};

