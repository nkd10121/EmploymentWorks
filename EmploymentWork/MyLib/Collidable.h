#pragma once
#include "GameObjectTag.h"
#include <List>
#include <memory>

namespace MyLib
{
	class Rigidbody;
	class Physics;
	class ColliderBase;

	/// <summary>
	/// �����蔻��N���X
	/// </summary>
	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:

		// �ʒu�␳�̗D��x�̔��ʂɎg��
		enum class Priority : int
		{
			Low,		// ��
			Middle,		// ��
			High,		// ��
			Static,		// �����Ȃ��i�ō��j
		};

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="priority">�D��x</param>
		/// <param name="tag">�^�O</param>
		Collidable(Priority priority, GameObjectTag tag);
		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~Collidable();
		/// <summary>
		/// ������(���g�ɓ����蔻���ǉ�)
		/// </summary>
		/// <param name="physics">�����N���X�|�C���^</param>
		virtual void Init(std::shared_ptr<MyLib::Physics> physics);
		/// <summary>
		/// �I��(���g�̓����蔻����폜)
		/// </summary>
		/// <param name="physics">�����N���X�|�C���^</param>
		virtual void Finalize(std::shared_ptr<MyLib::Physics> physics);

		//�����蔻��֐�
		virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnCollideStay(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnCollideExit(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerEnter(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerStay(const std::shared_ptr<Collidable>& colider) {}
		virtual void OnTriggerExit(const std::shared_ptr<Collidable>& colider) {}

		/// <summary>
		/// �^�O���擾
		/// </summary>
		/// <returns>�^�O</returns>
		GameObjectTag GetTag() const { return tag; }
		/// <summary>
		/// �D��x���擾
		/// </summary>
		/// <returns>�D��x</returns>
		Priority GetPriority() const { return priority; }

	protected:
		/// <summary>
		/// �����蔻���ǉ�
		/// </summary>
		/// <param name="kind">�����蔻��̎��</param>
		/// <param name="isTrigger">�����o����������邩�ǂ���</param>
		/// <returns>���g</returns>
		std::shared_ptr<ColliderBase> AddCollider(const ColliderBase::Kind& kind, bool isTrigger);

	protected:
		// �����f�[�^
		std::shared_ptr<Rigidbody> rigidbody;
		//�����蔻����
		std::list<std::shared_ptr<ColliderBase>> m_colliders;

	private:
		//�^�O
		GameObjectTag tag;
		//�D��x
		Priority priority;
	};
}