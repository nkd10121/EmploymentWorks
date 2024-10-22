#include "StateBase.h"
#include "CharacterBase.h"
#include <string>
#include <cassert>

/// <summary>
/// �R���X�g���N�^
/// </summary>
StateBase::StateBase(std::shared_ptr<CharacterBase> own):
	m_pOwn(own),
	m_nowState(StateKind::Null)
{
}

#ifdef _DEBUG
/// <summary>
/// ���݂̃X�e�[�g�̃f�o�b�O�`��
/// </summary>
void StateBase::DebugDrawState(int x, int y)
{
	std::string state = "null";

	switch (m_nowState)
	{
	case StateBase::StateKind::Idle:
		state = "Idle";
		break;
	case StateBase::StateKind::Walk:
		state = "Walk";
		break;
	case StateBase::StateKind::Dash:
		state = "Dash";
		break;
	case StateBase::StateKind::Jump:
		state = "Jump";
		break;
	case StateBase::StateKind::Attack:
		state = "Attack";
		break;
	case StateBase::StateKind::OnHit:
		state = "OnHit";
		break;
	case StateBase::StateKind::Death:
		state = "Death";
		break;
	default:
		state = "null";
		break;
	}

	DrawFormatString(x, y, 0xffffff, "%s", state.c_str());
}
#endif

/// <summary>
/// �����傪�v���C���[���ǂ����𔻒f����
/// </summary>
void StateBase::CheckPlayer()
{
	if (m_pOwn->GetTag() != GameObjectTag::Player)
	{
		assert(0 && "�����傪�v���C���[�ł͂���܂���");
	}
}