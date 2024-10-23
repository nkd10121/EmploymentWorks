#pragma once
#include "MyLib.h"
#include <map>
#include <memory>
#include <string>

/// <summary>
/// �Q�[�����̃L�����N�^�[�̊��N���X
/// </summary>
class CharacterBase : public MyLib::Collidable
{
public:
	//�X�e�[�^�X�\����
	struct Status
	{
		int hp;			//�̗�
		int atk;		//�U����
		int def;		//�h���
		float speed;	//�ړ����x
		int point;		//���S���̃|�C���g
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="priority">�D��x</param>
	/// <param name="tag">�^�O</param>
	CharacterBase(Priority priority, GameObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// �ق��̃I�u�W�F�N�g�ƏՓ˂����Ƃ��ɌĂ΂��
	/// </summary>
	/// <param name="colider">���������I�u�W�F�N�g</param>
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;


	/// <summary>
	/// ���g�̌��݂�HP��Ԃ�
	/// </summary>
	/// <returns>���݂�HP</returns>
	const int GetHp()const { return m_status.hp; }
	///// <summary>
	///// ���g�̍ő�HP��Ԃ�
	///// </summary>
	///// <returns>�ő�HP</returns>
	//const int GetMaxHp()const { return m_maxHp; }
	/// <summary>
	/// �����f�[�^��Ԃ�
	/// </summary>
	/// <returns>�����f�[�^</returns>
	std::shared_ptr<MyLib::Rigidbody> GetRigidbody(){ return rigidbody; }

protected:
	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="attachNo">�i�s���������A�j���[�V�����ԍ�</param>
	/// <returns>���[�v�������ǂ���</returns>
	bool UpdateAnim(int attachNo, float startTime = 0.0f);
	/// <summary>
	///	�A�j���[�V�����̕ύX
	/// </summary>
	/// <param name="animIndex">�ύX��̃A�j���[�V�����ԍ�</param>
	void ChangeAnim(int animIndex, float animSpeed = 0.5f);


protected:
	int m_modelHandle;	//���f���n���h��
	
	Status m_status;	//�X�e�[�^�X
	int m_hpMax;

	/*�A�j���[�V�����֌W*/
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;
	int m_equipAnimNo;			//1�O�̂̃A�j���[�V����
	int m_currentAnimNo;		//���݂̃A�j���[�V����
	int m_prevAnimNo;			//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;		//�A�j���[�V�����̍�������
	//0.0f:prev���Đ�
	//1.0f:currnt���Đ�

	float m_animSpeed;			//�A�j���[�V�����Đ����x
	bool m_isAnimationFinish;	//�A�j���[�V�������I��������ǂ���
};

