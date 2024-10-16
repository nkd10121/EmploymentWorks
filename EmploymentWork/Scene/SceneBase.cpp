#include "SceneBase.h"
#include "Game.h"

namespace
{
	// �t�F�[�h�ɂ�����t���[���f�t�H���g
	constexpr int kFadeFrameDefault = 30;

	constexpr int kFadeSpeed = 255 / kFadeFrameDefault;

	//���邳�̍ő�
	constexpr int kBrightMax = 255;
	//���邳�̍Œ�
	constexpr int kBrightMin = 0;
}

SceneBase::SceneBase() :
	m_fadeMask(-1),
	m_isInit(false),
	m_fadeBright(kBrightMax),
	m_fadeTotalFrame(kFadeFrameDefault),
	m_fadeSpeed(0),
	m_fadeColor(0x000000),
	m_fadePlayerKind(0),
	m_isDispLoading(false),
	m_loadFrame(0),
	m_isDispCursor(true),
	m_isEnd(false)
#ifdef DISP_PROCESS
	, m_updateTime(0),
	m_drawTime(0)
#endif
{
}

void SceneBase::EndThisScene()
{
	m_isEnd = true;
	StartFadeOut();
}

void SceneBase::InitAll()
{
	// �t�F�[�h�A�E�g��Ԃ���J�n
	m_fadeBright = kBrightMax;
	StartFadeIn();
	// �p����V�[���̏���������
	Init();
}

void SceneBase::EndAll()
{
	// �p����V�[���̏I������
	End();
}

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

bool SceneBase::IsSceneEnd()
{
	// ���������I���ƌ����Ă��Ȃ�
	if (!m_isEnd)	return false;
	// �܂��t�F�[�h�A�E�g�I����ĂȂ�
	if (m_fadeBright < kBrightMax)	return false;

	return true;
}

void SceneBase::UpdateFade()
{
	// ���\�[�X�̃��[�h���������Ă��Ȃ�
	if (m_fadeSpeed < 0 && (!IsLoaded()))
	{
		return;	// ���[�h�����܂ł̓t�F�[�h�C���������s��Ȃ�
	}

	m_fadeBright += m_fadeSpeed;
	if (m_fadeBright >= kBrightMax)
	{
		m_fadeBright = kBrightMax;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeBright <= kBrightMin)
	{
		m_fadeBright = kBrightMin;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

void SceneBase::DrawFade() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::DrawLoading() const
{
	if (!IsLoaded())
	{
		DrawFormatString(Game::kWindowWidth - 128, Game::kWindowHeight - 16,0xffffff,"Loading...");
	}
}

void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}
