#include "SpikeTrap.h"

#include "ModelManager.h"

namespace
{
	//当たり判定の円の半径
	constexpr float kCollisionRadius = 11.0f;


	constexpr float kModelScale = 0.08f;
}

SpikeTrap::SpikeTrap():
	TrapBase(GameObjectTag::SpikeTrap),
	m_spikeModel(-1)
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true,MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;
}


SpikeTrap::~SpikeTrap()
{
	MV1DeleteModel(m_spikeModel);
}


void SpikeTrap::Init(Vec3 pos)
{
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	rigidbody->SetPos(pos);
	rigidbody->SetNextPos(pos);


	//モデルのハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_SPIKEFRAME");
	MV1SetScale(m_modelHandle, VECTOR(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	m_spikeModel = ModelManager::GetInstance().GetModelHandle("M_SPIKE");
	MV1SetScale(m_spikeModel, VECTOR(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_spikeModel, pos.ToVECTOR());

	//存在フラグをtrueにする
	m_isExist = true;
}


void SpikeTrap::Update()
{

}


void SpikeTrap::Draw()
{
	MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_spikeModel);
}