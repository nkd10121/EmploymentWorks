#include "ScenePause.h"
#include "Game.h"
#include "SceneTitle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScenePause::ScenePause()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePause::~ScenePause()
{
}

/// <summary>
/// //���\�[�X�̃��[�h�J�n
/// </summary>
void ScenePause::StartLoad()
{
	//�������Ȃ�
}

/// <summary>
/// ���\�[�X�̃��[�h���I���������ǂ���
/// </summary>
bool ScenePause::IsLoaded() const
{
	//���̃V�[���̓��[�h�֌W��Init�ł���
	return true;
}

/// <summary>
/// ������
/// </summary>
void ScenePause::Init()
{
	//�t�F�[�h�A�E�g���X�L�b�v����
	SkipFadeOut();
}

/// <summary>
/// �I��
/// </summary>
void ScenePause::End()
{
}

/// <summary>
/// �X�V
/// </summary>
void ScenePause::Update()
{
	if(Input::GetInstance().IsTriggered("B"))
	{
		SceneManager::GetInstance().popScene();
		SkipFadeIn();
		return;
	}

	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}
}

/// <summary>
/// �`��
/// </summary>
void ScenePause::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(0 + 20, 0 + 20, Game::kWindowWidth - 20, Game::kWindowHeight - 20, 0xffffff, false);

#ifdef _DEBUG
	DrawString(0, 16, "PAUSE", 0xffffff);
#endif
}
