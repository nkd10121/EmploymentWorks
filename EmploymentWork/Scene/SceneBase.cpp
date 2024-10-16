#include "SceneBase.h"
#include "Game.h"

namespace
{
	// �t�F�[�h�ɂ�����t���[���f�t�H���g
	constexpr int kFadeFrameDefault = 30;
	// 1�t���[��������̃t�F�[�h���x
	constexpr int kFadeSpeed = 255 / kFadeFrameDefault;

	//���邳�̍ő�
	constexpr int kBrightMax = 255;
	//���邳�̍Œ�
	constexpr int kBrightMin = 0;
}

//�R���X�g���N�^
SceneBase::SceneBase() :
	m_isInit(false),
	m_isEnd(false),
	m_fadeAlpha(kBrightMax),
	m_fadeSpeed(0),
	m_fadeColor(0x000000)
#ifdef DISP_PROCESS
	, m_updateTime(0),
	m_drawTime(0)
#endif
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
void SceneBase::EndThisScene()
{
	m_isEnd = true;
	StartFadeOut();
}

/// <summary>
/// �h����̏������ƃV�[�����ʂŕK�v�ȏ��������s��
/// </summary>
void SceneBase::InitAll()
{
	// �t�F�[�h�A�E�g��Ԃ���J�n
	m_fadeAlpha = kBrightMax;
	StartFadeIn();
	// �p����V�[���̏���������
	Init();
}

/// <summary>
/// �h����̍X�V�ƃV�[�����ʂŕK�v�ȍX�V���s��
/// </summary>
void SceneBase::UpdateAll()
{
	//�p����̃V�[���̃��\�[�X�̃��[�h���I����Ă��邩�m�F
	if (!IsLoaded())
	{
		//���[�h���Ȃ�X�V���������s��Ȃ�
		return;
	}

	//�������֐����Ă񂾂��m�F
	if (!m_isInit)
	{
		//�Ă�ł��Ȃ������珉�����֐����Ă�
		InitAll();
		m_isInit = true;
	}

	UpdateFade();
	// �p����̃V�[���̍X�V����
	Update();
}

/// <summary>
/// �h����̕`��ƃV�[�����ʂŕK�v�ȕ`����s��
/// </summary>
void SceneBase::DrawAll()
{
#ifdef DISP_PROCESS
	LONGLONG start = GetNowHiPerformanceCount();
#endif

#ifdef TRUE
	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}
#endif

	//�p����̃V�[���̕`�揈��
	Draw();
	DrawFade();
	DrawLoading();

#ifdef DISP_PROCESS
	m_drawTime = GetNowHiPerformanceCount() - start;

	// �����o�[�̕\��
	// ���� 
	DrawString(0, Game::kWindowHeight - 48, "����", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kWindowHeight - 48 + 2, 48 + 16 - 2, Game::kWindowHeight - 32 - 2, 0x0000ff, true);
	DrawString(0, Game::kWindowHeight - 32, "�`��", 0xffffff, 0x000000);
	DrawBox(32 + 2, Game::kWindowHeight - 32 + 2, 48 + 16 - 2, Game::kWindowHeight - 16 - 2, 0xff0000, true);

	float rate = static_cast<float>(m_updateTime + m_drawTime) / 16666.6f;
	int width = static_cast<int>(Game::kWindowWidth * rate);
	DrawBox(0, Game::kWindowHeight - 16, width, Game::kWindowHeight, 0xff0000, true);	// ����+�`��

	rate = static_cast<float>(m_updateTime) / 16666.6f;
	width = static_cast<int>(Game::kWindowWidth * rate);
	DrawBox(0, Game::kWindowHeight - 16, width, Game::kWindowHeight, 0x0000ff, true);	// ����+�`��̏ォ�珈�����Ԃ̂ݕ`��
#endif
}

/// <summary>
/// ���݂̃V�[�������S�ɏI���������ǂ���
/// </summary>
bool SceneBase::IsSceneEnd()
{
	// ���������I���ƌ����Ă��Ȃ�
	if (!m_isEnd)	return false;
	// �܂��t�F�[�h�A�E�g�I����ĂȂ�
	if (m_fadeAlpha < kBrightMax)	return false;

	return true;
}

/// <summary>
/// �t�F�[�h�̍X�V
/// </summary>
void SceneBase::UpdateFade()
{
	// ���\�[�X�̃��[�h���������Ă��Ȃ�
	if (m_fadeSpeed < 0 && (!IsLoaded()))
	{
		return;	// ���[�h�����܂ł̓t�F�[�h�C���������s��Ȃ�
	}

	m_fadeAlpha += m_fadeSpeed;
	if (m_fadeAlpha >= kBrightMax)
	{
		m_fadeAlpha = kBrightMax;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeAlpha <= kBrightMin)
	{
		m_fadeAlpha = kBrightMin;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

/// <summary>
/// �t�F�[�h�̕`��
/// </summary>
void SceneBase::DrawFade() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ���[�h���`��
/// </summary>
void SceneBase::DrawLoading() const
{
	if (!IsLoaded())
	{
		DrawFormatString(Game::kWindowWidth - 128, Game::kWindowHeight - 16,0xffffff,"Loading...");
	}
}

/// <summary>
/// �t�F�[�h�C���J�n
/// </summary>
void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

/// <summary>
/// �t�F�[�h�A�E�g�J�n
/// </summary>
void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}

/// <summary>
/// �t�F�[�h�C�����X�L�b�v����
/// </summary>
void SceneBase::SkipFadeIn()
{
	m_fadeAlpha = kBrightMax;
	m_fadeSpeed = kFadeSpeed;
}

/// <summary>
/// �t�F�[�h�A�E�g���X�L�b�v����
/// </summary>
void SceneBase::SkipFadeOut()
{
	m_fadeAlpha = kBrightMin;
	m_fadeSpeed = -kFadeSpeed;
}
