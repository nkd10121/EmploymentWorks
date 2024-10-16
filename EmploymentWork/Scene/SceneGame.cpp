#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"

SceneGame::SceneGame():
	temp_handle()
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::StartLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	// TODO:���̊ԂŃ��\�[�X�����[�h����
	for (int i = 0; i < 5; i++)
	{
		temp_handle.push_back(MV1LoadModel("data/model/player.mv1"));
	}

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

bool SceneGame::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����
	for (auto& h : temp_handle)
	{
		if (CheckHandleASyncLoad(h))	return false;
	}

	return true;
}

void SceneGame::Init()
{
	//TODO:�����Ŏ��Ԃ̐����Ȃǂ�����
}

void SceneGame::End()
{
	//TODO:�����Ń��\�[�X�̃������J���Ȃǂ�����
	for (auto& h : temp_handle)
	{
		MV1DeleteModel(h);
	}
	temp_handle.clear();
}

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

void SceneGame::Draw()
{
	for (auto& h : temp_handle)
	{
		MV1DrawModel(h);
	}

#ifdef _DEBUG
	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
