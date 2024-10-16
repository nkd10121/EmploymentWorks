#include "SceneTitle.h"
#include "SceneGame.h"

SceneTitle::SceneTitle()
{

}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::StartLoad()
{
	// �񓯊��ǂݍ��݂��J�n����
	SetUseASyncLoadFlag(true);

	//TODO:���̊ԂŃ��\�[�X�����[�h����

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

bool SceneTitle::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����
	//if (CheckHandleASyncLoad(m_bgTile))	return false;

	return true;
}

void SceneTitle::Init()
{
	//TODO:�����Ŏ��Ԃ̐����Ȃǂ�����
}

void SceneTitle::End()
{
	//TODO:�����Ń��\�[�X�̃������J���Ȃǂ�����
}

void SceneTitle::Update()
{
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneGame>());
		EndThisScene();
		return;
	}
}

void SceneTitle::Draw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TITLE", 0xffffff);
#endif
}
