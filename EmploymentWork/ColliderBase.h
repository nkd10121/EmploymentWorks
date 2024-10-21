#pragma once
#include <unordered_map>
#include "Vec3.h"

namespace MyLib
{
	/// <summary>
	/// �����蔻��̃f�[�^���
	/// </summary>
	class ColliderBase abstract
	{
	public:
		// �����蔻����
		enum class Kind
		{
			Sphere,		//��
			Cupsule,	//�J�v�Z��
		};

		// �R���X�g���N�^
		ColliderBase(Kind kind, bool isTrigger)
		{
			this->kind = kind;
			this->isTrigger = isTrigger;
		}

		virtual ~ColliderBase() {}

		// �����蔻���ʎ擾
		Kind GetKind() const { return kind; }

		// �g���K�[���ǂ���
		bool IsTrigger() const { return isTrigger; }

	private:
		Kind	kind;
		bool	isTrigger;
	};
}
