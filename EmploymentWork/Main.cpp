#include "DxLib.h"
#include "Util/Game.h"
#include "Camera.h"
#include "Input.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include <memory>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(true);
	//�E�B���h�E�T�C�Y�̕ύX
	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 32);
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kWindowName);
	//�t���X�N���[���؂�ւ����Ƀ��\�[�X�n���h�������Z�b�g���Ȃ��悤�ɐݒ肷��
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());


	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		//�V�[���̍X�V
		SceneManager::GetInstance().Update();

		//�V�[���̕`��
		SceneManager::GetInstance().Draw();

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	//static�N���X�̍폜
	SceneManager::GetInstance().Destroy();
	Input::GetInstance().Destroy();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}