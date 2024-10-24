#pragma once
#include <unordered_map>
#include "Vec3.h"

namespace MyLib
{
	/// <summary>
	/// �����蔻��̃f�[�^���N���X
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

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="kind">�����蔻����</param>
		/// <param name="isTrigger">�ʒu�␳�����邩�ǂ����@false : ���Ȃ�, true : ����</param>
		ColliderBase(Kind kind, bool isTrigger);
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~ColliderBase() {}

		/// <summary>
		/// �����蔻��̎�ʎ擾
		/// </summary>
		/// <returns>�����蔻��̎��</returns>
		Kind GetKind() const { return kind; }

		/// <summary>
		/// �g���K�[���ǂ������擾
		/// </summary>
		/// <returns>�g���K�[�t���O</returns>
		bool IsTrigger() const { return isTrigger; }

	private:
		Kind kind;		//�����蔻����
		bool isTrigger;	//�g���K�[�t���O
	};
}
