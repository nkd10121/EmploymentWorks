#pragma once
#include "ObjectBase.h"

class HealPortion : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	HealPortion();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~HealPortion();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="physics">�����N���X</param>
	void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// �����蔻����폜
	/// </summary>
	/// <param name="physics">�����N���X</param>
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���݂��Ă��邩�ǂ������擾
	/// </summary>
	/// <returns></returns>
	const bool GetIsExist()const { return m_isExist; }
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="pos">�ݒ���W</param>
	void SetPosition(Vec3 pos);
	/// <summary>
	/// �I��������
	/// </summary>
	void End();

private:
	bool m_isExist;	//���݃t���O
	int m_effectCreateFrame;	//�t���[����

	float m_angle;	//�p�x
	float m_posOffsetY;	//������
};

