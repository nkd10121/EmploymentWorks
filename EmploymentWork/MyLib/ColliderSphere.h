#pragma once
#include "ColliderBase.h"

namespace MyLib
{
	/// <summary>
	/// ���^�̓����蔻��
	/// </summary>
	class ColliderSphere : public ColliderBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="isTrigger">�g���K�[���ǂ���</param>
		ColliderSphere(bool isTrigger);

	public:
		float m_radius;	//���a
	};
}
