#pragma once

//���ꂼ��̃V�[����include����̂߂�ǂ������U������include
//�悭�Ȃ����Č���ꂽ�炻�ꂼ��̃V�[���ɃR�s�y����
#include "SceneManager.h"
#include "Input.h"
#include "DxLib.h"

#ifdef _DEBUG
// �������׌v��
#define DISP_PROCESS
#endif

/// <summary>
/// �V�[���̊��N���X
/// </summary>
class SceneBase
{
public:
	SceneBase();
	virtual ~SceneBase() {}

public:	/*�h���N���X�Ɍp������K�v�̂��鏈��*/
	/// <summary>
	/// //���\�[�X�̃��[�h�J�n
	/// </summary>
	virtual void StartLoad() = 0;
	/// <summary>
	/// ���\�[�X�̃��[�h���I���������ǂ���
	/// </summary>
	/// <returns>true : �I���ς�, false : �I�����Ă��Ȃ�</returns>
	virtual bool IsLoaded()const = 0;

	virtual void Init() = 0;	// ������	���\�[�X�̃��[�h������ɌĂ΂��
	virtual void End() = 0;		// �V�[���I�����ɌĂ΂��	���\�[�X�̉���͂���

	virtual void Update() = 0;	// ���\�[�X�̃��[�h�����ȍ~�Ă΂��	�t�F�[�h�����Ă΂��
	virtual void Draw() = 0;

#ifdef _DEBUG
	// �f�o�b�O�\��(�őO�ʕ\��)
	virtual void drawDebug() {}
#endif

protected:
	/// <summary>
	/// ���݂̃V�[�����I��������
	/// </summary>
	void EndThisScene();
public:
	// �p�����s��Ȃ�����	SceneManager����Ăяo���̂͂�����
	void InitAll();	// �V�[���ʂ�init()�̑����ʂŕK�v�ȏ��������s��	
	// ����̓��[�h������̏���update()�ŌĂԂ̂�SceneManager()����͌Ă΂Ȃ�
	void EndAll();	// �V�[���ʂ�end()�̑����ʂŕK�v�ȏ��������s��

	void UpdateAll();	// �V�[���ʂ�update()�̑����ʂŕK�v�ȏ��������s��
	void DrawAll();		// �V�[���ʂ�draw()�̑����ʂŕK�v�ȏ��������s��

	bool IsSceneEnd();	// ���݂̃V�[�����I�����Ď��̃V�[���ɑJ�ڂ���

private:	/*�t�F�[�h�֌W*/
	void UpdateFade();
	void DrawFade() const;

	// ���[�h���\��
	void DrawLoading() const;

	/// <summary>
	/// �t�F�[�h�C���J�n
	/// </summary>
	void StartFadeIn();
	/// <summary>
	/// �t�F�[�h�A�E�g�J�n
	/// </summary>
	void StartFadeOut();	

private:

	int m_fadeMask;

	// �����������I������
	bool m_isInit;

	// �t�F�[�h�֘A����
	int m_fadeBright;	// 0:�t�F�[�h�C������	=m_fadeTotalFrame:��ʐ^����
	int m_fadeTotalFrame;
	int m_fadeSpeed;

	// �t�F�[�h���o
	unsigned int m_fadeColor;
	int m_fadePlayerKind;


	// ���[�h���\�����s����
	bool m_isDispLoading;
	// ���[�h�t���[�����J�E���g
	int m_loadFrame;

	// �J�[�\���\��
	bool m_isDispCursor;

	// �V�[���J��
	bool m_isEnd;

	// �������׌v��
#ifdef DISP_PROCESS
	LONGLONG	m_updateTime;	// update�ɂ�����������(�~���b)
	LONGLONG	m_drawTime;		// draw�ɂ�����������(�~���b)
#endif
};

