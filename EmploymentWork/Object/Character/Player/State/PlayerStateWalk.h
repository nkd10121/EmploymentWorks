#pragma once
#include "StateBase.h"

/// <summary>
/// �v���C���[�̕������
/// </summary>
class PlayerStateWalk : public StateBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="own"></param>
	PlayerStateWalk(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() override;

	//virtual int OnDamage(std::shared_ptr<MyLib::Collidable> collider) override;
};