#pragma once
#include "StateBase.h"

/// <summary>
/// �v���C���[�̑ҋ@���
/// </summary>
class PlayerStateIdle : public StateBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="own">������̃|�C���^</param>
	PlayerStateIdle(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() override;
	///// <summary>
	///// ��_���[�W
	///// </summary>
	///// <param name="collider">���������I�u�W�F�N�g</param>
	///// <returns>�_���[�W��?</returns>
	//virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;
};