#pragma once
#include "SceneBase.h"
#include "Camera.h"
#include "MyLib.h"
//�e�X�g�p
#include <list>

//�v���g�^�C�v�錾
class Player;	//�v���C���[
class HealPortion;	//�񕜃|�[�V����

/// <summary>
/// �Q�[���V�[��
/// </summary>
class SceneGame : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneGame();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneGame();

	/// <summary>
	/// //���\�[�X�̃��[�h�J�n
	/// </summary>
	virtual void StartLoad() override;

	/// <summary>
	/// ���\�[�X�̃��[�h���I���������ǂ���
	/// </summary>
	/// <returns>true : �I���ς�, false : �I�����Ă��Ȃ�</returns>
	virtual bool IsLoaded() const override;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() override;

	/// <summary>
	/// �I��
	/// </summary>
	virtual void End() override;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() override;

private:
	std::shared_ptr<Player> m_pPlayer;			//�v���C���[�|�C���^
	std::shared_ptr<Camera> m_pCamera;			//�J�����|�C���^
	std::shared_ptr<MyLib::Physics> m_pPhysics;	//�����N���X�|�C���^
	std::list<std::shared_ptr<HealPortion>> m_pPortions;	//�񕜃|�[�V�����|�C���^�̔z��

	int m_stageModel;	//���f���n���h��(��)

};

