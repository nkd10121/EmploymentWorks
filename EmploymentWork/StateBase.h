#pragma once
#include <memory>
#include "MyLib.h"

class CharacterBase;

/// <summary>
/// �X�e�[�g�p�^�[���̊��N���X
/// </summary>
class StateBase : public std::enable_shared_from_this<StateBase>
{
public:
	//�X�e�[�g
	enum class StateKind
	{
		Null,
		Idle,		//�ҋ@
		Walk,		//�ړ�
		Dash,		//�_�b�V��
		Jump,		//�W�����v
		Attack,		//�U��
		OnHit,		//��_���[�W
		Death,		//���S
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="own">���L��</param>
	StateBase(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// ���݂�State��Ԃ�
	/// </summary>
	/// <returns>���݂�State</returns>
	StateKind GetKind() { return m_nowState; }
	/// <summary>
	/// �J�ڗ\���State��Ԃ�
	/// </summary>
	/// <returns>�J�ڗ\���State</returns>
	const std::shared_ptr<StateBase> GetNextState()const { return m_nextState; }

	/// <summary>
	/// �J�ڗ\���State��ݒ肷��
	/// </summary>
	/// <param name="state">�J�ڗ\���State</param>
	void SetNextState(std::shared_ptr<StateBase> state) { m_nextState = state; }

	/// <summary>
	/// �_���[�W���󂯂����̏������s��
	/// </summary>
	/// <param name="collider"></param>
	/// <returns>�_���[�W</returns>
	virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) abstract;



#ifdef _DEBUG
	/// <summary>
	/// ���݂̃X�e�[�g�̃f�o�b�O�`��
	/// </summary>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	void DebugDrawState(int x,int y);
#endif

protected:
	/// <summary>
	/// �����傪�v���C���[���ǂ����𔻒f����
	/// </summary>
	void CheckPlayer();

protected:
	std::shared_ptr<CharacterBase> m_pOwn;	//���L�҂̃|�C���^
	std::shared_ptr<StateBase> m_nextState;	//�J�ڐ��State��ۑ�����

	StateKind m_nowState;					//���݂̃X�e�[�g
};

