#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneGame::SceneGame():
	temp_handle(),
	m_pPhysics(nullptr)
{
	m_pPlayer = std::make_shared<Player>();

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

	m_pPhysics = std::make_shared<MyLib::Physics>();
	m_pPlayer->Init(m_pPhysics);

	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();


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
#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

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

	m_pCamera->Update();

	m_pPlayer->Update();

	// �����X�V
	m_pPhysics->Update();
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

	m_pPlayer->Draw();

#ifdef _DEBUG
	MyLib::DebugDraw::Draw3D();

	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
