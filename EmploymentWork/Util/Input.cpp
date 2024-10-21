#include "Input.h"
#include <cassert>

Input* Input::m_instance = nullptr;

namespace
{

}

enum PadCheckMask : int
{
	A = 0x00000010,	//A�{�^��
	B = 0x00000020,	//B�{�^��
	X = 0x00000040,	//X�{�^��
	Y = 0x00000080,	//Y�{�^��

	L = 0x00000100,	//L�{�^��
	R = 0x00000200,	//R�{�^��

	RStickPush = 0x00002000,	//R�X�e�B�b�N��������
	test = 0x00008000,	//R�X�e�B�b�N��������

	P = 0x00000800,	//�z�[���{�^��(?)

	UP = 0x00000008,	//��{�^��
	RIGHT = 0x00000004,	//�E�{�^��
	DOWN = 0x00000001,	//���{�^��
	LEFT = 0x00000002,	//���{�^��
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
Input::Input():
	m_padState()
{
	m_commandTable["A"] = { PadCheckMask::A };
	m_commandTable["B"] = { PadCheckMask::B };
	m_commandTable["X"] = { PadCheckMask::X };
	m_commandTable["Y"] = { PadCheckMask::Y };
}

/// <summary>
/// ���͏����X�V����
/// </summary>
void Input::Update()
{
	m_lastInputData = m_inputData;	//���O���͂��R�s�[���Ă���(�������u�Ԃ��擾����p)

	//�n�[�h�E�F�A���̓`�F�b�N
	char keystate[256];
	GetHitKeyStateAll(keystate);//���݂̃L�[�{�[�h���͂��擾
	int padstate = GetJoypadInputState(DX_INPUT_PAD1);//�p�b�h���̎擾

	//�o�^���ꂽ���ƃn�[�h�̏����Ƃ炵���킹�Ȃ���
	//inputData_�̓��e���X�V���Ă���
	for (const auto& cmd : m_commandTable)
	{
		auto& input = m_inputData[cmd.first];//�R�}���h�̖��O������̓f�[�^�����

		//�L�[�{�[�h�̃`�F�b�N
		input = false;
		if (padstate & cmd.second)
		{
			input = true;
			break;
		}
	}

	//�{�^���̓��͂��擾
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_padState);
}

/// <summary>
/// �w��̃R�}���h�������ꂽ�u�ԂȂ̂�
/// </summary>
bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())	//��񂪂Ȃ����false��Ԃ�
	{
		assert(0 && "�w�肳�ꂽ�R�}���h�͓o�^����Ă��܂���");
		return false;
	}
	return (m_inputData.at(command) && !m_lastInputData.at(command));
}

/// <summary>
/// �w��̃R�}���h�������������Ă��邩�ǂ���
/// </summary>
bool Input::IsPushed(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())	//��񂪂Ȃ����false��Ԃ�
	{
		assert(0 && "�w�肳�ꂽ�R�}���h�͓o�^����Ă��܂���");
		return false;
	}
	return (m_inputData.at(command) && m_lastInputData.at(command));
}

std::pair<float, float> Input::GetInputStick(bool isRight) const
{
	if (isRight)
	{
		//�E�X�e�B�b�N�̓��͏���Ԃ�
		return std::make_pair(static_cast<float>(m_padState.Rx), static_cast<float>(m_padState.Ry));
	}
	else
	{
		//���X�e�B�b�N�̓��͏���Ԃ�
		return std::make_pair(static_cast<float>(m_padState.X), static_cast<float>(m_padState.Y));
	}
}
