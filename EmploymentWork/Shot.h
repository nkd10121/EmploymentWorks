#pragma once
#include "ObjectBase.h"

/// <summary>
/// �������U���̒e�N���X
/// </summary>
class Shot : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="tag">�����蔻��^�O</param>
	Shot(GameObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Shot();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="physics">�����N���X�|�C���^</param>
	void Init(std::shared_ptr<MyLib::Physics> physics);
	/// <summary>
	/// �����蔻����폜
	/// </summary>
	/// <param name="physics">�����N���X�|�C���^</param>
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	/// <summary>
	/// �������W�ƌ����ƍU���͂�ݒ�
	/// </summary>
	/// <param name="pos">�������W</param>
	/// <param name="m_dir">����</param>
	/// <param name="atk">�U����</param>
	void Set(const Vec3& pos, const Vec3& m_dir, const int& atk);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I���ʒm
	/// ���������I�u�W�F�N�g����Ă΂��
	/// </summary>
	void End();

	const bool GetIsExist()const { return m_isExist; }
	const int GetAtk()const { return m_atk; }

private:
	Vec3 m_moveDir;
	bool m_isExist;

	int m_frameCount;

	float m_angle;
	Vec3 m_rot;

	int m_mapHandle;

	int m_atk;
};