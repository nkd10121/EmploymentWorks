#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"
#include "HealPortion.h"

#include "ModelManager.h"
#include "MapManager.h"
#include "LoadCSV.h"

namespace
{
	//�|�[�V�������f���̃p�X
	const std::string kPortionPath = "data/model/object/portion/bottle_red.mv1";
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneGame::SceneGame():
	m_pPlayer(nullptr),
	m_pCamera(nullptr),
	m_pPhysics(nullptr),
	m_pPortions()
{
	//���̃V�[���Ń��[�h����ׂ����\�[�X�̃p�X���擾
	m_loadPath = LoadCSV::GetInstance().GetLoadResourcePath("SCENE_GAME");
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

	for (auto& path : m_loadPath)
	{
		//���f���f�[�^�Ȃ�
		if (path.second == ".mv1")
		{
			auto p = path.first + path.second;
			ModelManager::GetInstance().LoadModel(p);
		}
	}


	//MapManager::GetInstance().LoadModel();

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// ���\�[�X�̃��[�h���I���������ǂ���
/// </summary>
bool SceneGame::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����

	if (!ModelManager::GetInstance().IsLoaded())	return false;

	return true;
}

/// <summary>
/// ������
/// </summary>
void SceneGame::Init()
{
	//TODO:�����Ŏ��Ԃ̐����Ȃǂ�����
	m_pPhysics = std::make_shared<MyLib::Physics>();

	//�v���C���[�̐���
	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init(m_pPhysics);

	//�J�����̐���
	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	//m_pPortions.emplace_back(std::make_shared<HealPortion>());
	//m_pPortions.back()->Init(m_pPhysics);
	//m_pPortions.back()->SetPosition(Vec3(0.0f,0.0f,-10.0f));

	//�X�e�[�W�������[�h
	MapManager::GetInstance().Init();
	MapManager::GetInstance().Load("data");
}

/// <summary>
/// �I��
/// </summary>
void SceneGame::End()
{
	//TODO:�����Ń��\�[�X�̊J��������

	//�|�[�V�����̉��
	for (auto& p : m_pPortions)
	{
		p->Finalize(m_pPhysics);
	}
	m_pPortions.clear();
}

/// <summary>
/// �X�V
/// </summary>
void SceneGame::Update()
{
#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif
	//X�{�^������������^�C�g����ʂɖ߂�悤��
	if (Input::GetInstance().IsTriggered("X"))
	{
		SceneManager::GetInstance().ChangeScene(std::make_shared<SceneTitle>());
		EndThisScene();
		return;
	}

	//Y�{�^������������|�[�Y��ʂ��J���悤��
	if (Input::GetInstance().IsTriggered("Y"))
	{
		SceneManager::GetInstance().PushScene(std::make_shared<ScenePause>());
		return;
	}

	//DEBUG:A�{�^�������������Ƀ|�[�V�����𐶐�����悤��
	if (Input::GetInstance().IsTriggered("A"))
	{
		m_pPortions.emplace_back(std::make_shared<HealPortion>());
		m_pPortions.back()->Init(m_pPhysics);
		m_pPortions.back()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
	}

	//�v���C���[�̍X�V
	m_pPlayer->SetCameraAngle(m_pCamera->GetDirection());
	m_pPlayer->Update();

	//�J�����̍X�V
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->Update();

	//�|�[�V�����̍X�V
	for (auto& p : m_pPortions)
	{
		p->Update();

		if (!p->GetIsExist())
		{
			p->Finalize(m_pPhysics);
		}
	}
	//isExist��false�̃I�u�W�F�N�g���폜
	{
		auto it = std::remove_if(m_pPortions.begin(), m_pPortions.end(), [](auto& v)
			{
				return v->GetIsExist() == false;
			});
		m_pPortions.erase(it, m_pPortions.end());
	}

	//�����X�V
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

	//�X�e�[�W�̕`��
	MapManager::GetInstance().Draw();

	//�v���C���[�̕`��
	m_pPlayer->Draw();

	//�|�[�V�����̕`��
	for (auto& p : m_pPortions)
	{
		p->Draw();
	}

#ifdef _DEBUG
	MyLib::DebugDraw::Draw3D();

	DrawString(0, 0, "GAME", 0xffffff);
#endif
}
