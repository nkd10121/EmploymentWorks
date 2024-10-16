#include "SceneTitle.h"
#include "SceneGame.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneTitle::SceneTitle()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneTitle::~SceneTitle()
{
}

/// <summary>
/// //���\�[�X�̃��[�h�J�n
/// </summary>
void SceneTitle::StartLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	//TODO:���̊ԂŃ��\�[�X�����[�h����

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// ���\�[�X�̃��[�h���I���������ǂ���
/// </summary>
bool SceneTitle::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����
	//if (CheckHandleASyncLoad(m_bgTile))	return false;

	return true;
}

/// <summary>
/// ������
/// </summary>
void SceneTitle::Init()
{
	//TODO:�����Ŏ��Ԃ̐����Ȃǂ�����
}

/// <summary>
/// �I��
/// </summary>
void SceneTitle::End()
{
	//TODO:�����Ń��\�[�X�̃������J���Ȃǂ�����
}

/// <summary>
/// �X�V
/// </summary>
void SceneTitle::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}
}

/// <summary>
/// �`��
/// </summary>
void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TITLE", 0xffffff);
#endif
}
