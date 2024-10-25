#include "Shot.h"

#include "ModelManager.h"
#include "MapManager.h"

namespace
{
	//�����蔻��̔��a
	constexpr float kCollisionRadius = 1.0f;

	//�p�X
	const std::string kPath = "data/model/arrow.mv1";
}

Shot::Shot(GameObjectTag tag):
	ObjectBase(Collidable::Priority::Low, tag),
	m_moveDir(),
	m_isExist(false),
	m_frameCount(0),
	m_atk(0)
{
	//�����蔻��̐���
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;
}

Shot::~Shot()
{

}

void Shot::Init(std::shared_ptr<MyLib::Physics> physics)
{

	//�����蔻��̏�����
	Collidable::Init(physics);
	//�����N���X�̏�����
	rigidbody->Init();

	//���f���n���h�����擾
	m_modelHandle = ModelManager::GetInstance().GetModelHandle(kPath);
	MV1SetScale(m_modelHandle, VECTOR(8.0f, 8.0f, 8.0f));

	//�}�b�v�̓����蔻�胂�f���̃n���h�����擾
	//m_mapHandle = MapManager::GetInstance().GetStageCollisionHandle();

	//���݃t���O��true�ɂ���
	m_isExist = true;
}

void Shot::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Finalize(physics);
}

void Shot::Set(const Vec3& pos, const Vec3& m_dir, const int& atk)
{
	rigidbody->SetPos(pos + m_dir);
	m_moveDir = m_dir;
	m_atk = atk;

	//��]������������v�Z����
	m_angle = -atan2f(m_moveDir.z, m_moveDir.x) - DX_PI_F / 2;
	m_rot = Vec3(0.0f, m_angle, 0.0f);

	MV1SetRotationXYZ(m_modelHandle, m_rot.ToVECTOR());
}

void Shot::Update()
{
	//���݂��Ă��Ȃ���ԂȂ牽�������Ȃ�
	if (!m_isExist)return;

	m_frameCount++;

	auto dirNorm = m_moveDir.Normalize();
	rigidbody->SetVelocity(dirNorm * 1.0f);

	auto hitDim = MV1CollCheck_Sphere(m_mapHandle, -1, rigidbody->GetPosVECTOR(), kCollisionRadius);

	// ���o�������͂̃|���S�������J������
	MV1CollResultPolyDimTerminate(hitDim);

	if (hitDim.HitNum != 0)
	{
		// ���o�������͂̃|���S�������J������
		m_isExist = false;
	}

	if (m_frameCount > 300)
	{
		m_isExist = false;
	}
}

void Shot::Draw()
{
	//���݂��Ă��Ȃ���ԂȂ牽�������Ȃ�
	if (!m_isExist)return;

	rigidbody->SetPos(rigidbody->GetNextPos());
	auto pos = rigidbody->GetPos();
	MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	MV1DrawModel(m_modelHandle);
}

void Shot::End()
{
	m_isExist = false;
}