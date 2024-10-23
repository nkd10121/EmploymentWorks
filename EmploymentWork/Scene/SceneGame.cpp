#include "SceneGame.h"
#include "SceneTitle.h"
#include "ScenePause.h"
#include "Player.h"
#include "HealPortion.h"
#include "ModelManager.h"

namespace
{
	//�X�e�[�W�p�X
	const std::string kStagePath = "data/model/temp_stage1.mv1";
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
	m_modelHandles()
{

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

	ModelManager::GetInstance().LoadModel(kStagePath);
	ModelManager::GetInstance().LoadModel(kPortionPath);

	// �f�t�H���g�ɖ߂�
	SetUseASyncLoadFlag(false);
}

/// <summary>
/// ���\�[�X�̃��[�h���I���������ǂ���
/// </summary>
bool SceneGame::IsLoaded() const
{
	//TODO:�����Ń��\�[�X�����[�h�����ǂ����𔻒f����

	auto modelHandle = ModelManager::GetInstance().GetPrimitiveModelHandles();
	for (auto& h : modelHandle)
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

	m_pPlayer = std::make_shared<Player>();

	m_pPhysics = std::make_shared<MyLib::Physics>();
	m_pPlayer->Init(m_pPhysics);

	m_pCamera = std::make_shared<Camera>();
	m_pCamera->Init();

	m_modelHandles.emplace_back(ModelManager::GetInstance().GetModelHandle(kStagePath));
	MV1SetScale(m_modelHandles.back(), VGet(0.01f, 0.01f, 0.01f));

	m_pPortions.emplace_back(std::make_shared<HealPortion>());
	m_pPortions.back()->Init(m_pPhysics);
	m_pPortions.back()->SetPosition(MyLib::Vec3(0.0f,0.0f,-10.0f));
}

/// <summary>
/// �I��
/// </summary>
void SceneGame::End()
{
	//TODO:�����Ń��\�[�X�̊J��������

	for (auto& h : m_modelHandles)
	{
		MV1DeleteModel(h);
	}
	m_modelHandles.clear();
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

	for (auto& h : m_modelHandles)
	{
		MV1DrawModel(h);
	}

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
