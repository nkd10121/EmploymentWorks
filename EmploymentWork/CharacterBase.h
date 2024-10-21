#pragma once
#include "MyLib.h"
#include <map>
#include <memory>
#include <string>

class CharacterBase : public MyLib::Collidable
{
public:
	//�X�e�[�^�X�\����
	struct Status
	{
		int hp;		//�̗�
		int atk;	//�U����
		int def;	//�h���
		float speed;	//�ړ����x
		int point;	//���S���̃|�C���g
	};

public:
	CharacterBase(Priority priority, GameObjectTag tag);
	virtual ~CharacterBase();

	const int GetHp()const { return m_status.hp; }
	const int GetMaxHp()const { return m_maxHp; }

	// �Փ˂����Ƃ�
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:
	//�A�j���[�V�����̍X�V
	bool UpdateAnim(int attachNo, float startTime = 0.0f);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex, float animSpeed = 0.5f);

	//�X�e�[�^�X
	Status m_status;
	int m_maxHp;

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

