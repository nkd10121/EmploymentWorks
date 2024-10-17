#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneGame::SceneGame():
	temp_handle()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneGame::~SceneGame()
{
}

/// <summary>
/// //���\�[�X�̃��[�h�J�n
/// </summary>
void SceneGame::StartLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	// TODO:���̊ԂŃ��\�[�X�����[�h����
	for (int i = 0; i < 1; i++)
	{
		temp_handle.push_back(MV1LoadModel("data/model/temp_stage1.mv1"));
	}

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// ���\�[�X�̃��[�h���I���������ǂ���
/// </summary>
bool SceneGame::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����
	for (auto& h : temp_handle)
	{
		if (CheckHandleASyncLoad(h))	return false;
	}

	return true;
}

/// <summary>
/// ������
/// </summary>
void SceneGame::Init()
{
	//TODO:�����Ŏ��Ԃ̐����Ȃǂ�����
	for (auto& h : temp_handle)
	{
		MV1SetScale(h, VGet(0.01f, 0.01f, 0.01f));
	}
}

/// <summary>
/// �I��
/// </summary>
void SceneGame::End()
{
	//TODO:�����Ń��\�[�X�̃������J���Ȃǂ�����
	for (auto& h : temp_handle)
	{
		MV1DeleteModel(h);
	}
	temp_handle.clear();
}

/// <summary>
/// �X�V
/// </summary>
void SceneGame::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}

	if (Input::GetInstance().IsTriggered("Y"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}
}

/// <summary>
/// �`��
/// </summary>
void SceneGame::Draw()
{
	// ���\�[�X�̃��[�h���I���܂ł͕`�悵�Ȃ��̂��悳����
	// (�ǂ���ɂ���t�F�[�h�d�؂��Ă���̂ŉ��������Ȃ��͂�)
	if (!IsLoaded())	return;
	if (!IsInitialized())	return;

	for (auto& h : temp_handle)
	{
		MV1DrawModel(h);
	}

#ifdef _DEBUG
	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
