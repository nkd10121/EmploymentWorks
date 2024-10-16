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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneBase();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
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

	/// <summary>
	/// ������
	/// MEMO:���\�[�X�̃��[�h������ɌĂ΂��
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �I��
	/// MEMO:���\�[�X�̉���͂��̊֐����ōs��
	/// </summary>
	virtual void End() = 0;

	/// <summary>
	/// �X�V
	/// MEMO:�t�F�[�h�����Ă΂��
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
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

public:	/*�p�����s��Ȃ�����	SceneManager����Ăяo���̂͂�����*/

	/// <summary>
	/// �h����̏������ƃV�[�����ʂŕK�v�ȏ��������s��
	/// MEMO:����̓��[�h������̏���update()�ŌĂԂ̂�SceneManager()����͌Ă΂Ȃ�
	/// </summary>
	void InitAll();

	/// <summary>
	/// �h����̍X�V�ƃV�[�����ʂŕK�v�ȍX�V���s��
	/// </summary>
	void UpdateAll();

	/// <summary>
	/// �h����̕`��ƃV�[�����ʂŕK�v�ȕ`����s��
	/// </summary>
	void DrawAll();

	/// <summary>
	/// ���݂̃V�[�������S�ɏI���������ǂ���
	/// </summary>
	/// <returns>true : �I������, false : ���������I������\�肪�Ȃ� or �t�F�[�h��</returns>
	bool IsSceneEnd();

private:	/*�t�F�[�h�֌W*/

	/// <summary>
	/// �t�F�[�h�̍X�V
	/// </summary>
	void UpdateFade();

	/// <summary>
	/// �t�F�[�h�̕`��
	/// </summary>
	void DrawFade() const;

	/// <summary>
	/// ���[�h���`��
	/// </summary>
	void DrawLoading() const;

	/// <summary>
	/// �t�F�[�h�C���J�n
	/// </summary>
	void StartFadeIn();
	/// <summary>
	/// �t�F�[�h�A�E�g�J�n
	/// </summary>
	void StartFadeOut();	

protected:

	/// <summary>
	/// �t�F�[�h�C�����X�L�b�v����
	/// </summary>
	void SkipFadeIn();

	/// <summary>
	/// �t�F�[�h�A�E�g���X�L�b�v����
	/// </summary>
	void SkipFadeOut();
private:
	bool m_isInit;	//�����������I������
	bool m_isEnd;	//���̃V�[���ɑJ�ڂ���

	// �t�F�[�h�֘A����
	int m_fadeAlpha;	//�t�F�[�h�̃A���t�@�l
						//0:�t�F�[�h������Ȃ�,0�ȏ�:�t�F�[�h��
	int m_fadeSpeed;	//�t�F�[�h���x
	unsigned int m_fadeColor;	//�t�F�[�h���̐F

#ifdef DISP_PROCESS
	/*�������׌v��*/
	LONGLONG	m_updateTime;	// update�ɂ�����������(�~���b)
	LONGLONG	m_drawTime;		// draw�ɂ�����������(�~���b)
#endif
};

