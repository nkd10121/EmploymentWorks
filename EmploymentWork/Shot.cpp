#include "Shot.h"

#include "ModelManager.h"
#include "MapManager.h"

namespace
{
	//当たり判定の半径
	constexpr float kCollisionRadius = 1.0f;

	//パス
	const std::string kPath = "data/model/arrow.mv1";
}

Shot::Shot(GameObjectTag tag):
	ObjectBase(Collidable::Priority::Low, tag),
	m_moveDir(),
	m_frameCount(0),
	m_atk(0)
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;
}

Shot::~Shot()
{

}

void Shot::Init(std::shared_ptr<MyLib::Physics> physics)
{
	//当たり判定の初期化
	Collidable::Init(physics);
	//物理クラスの初期化
	rigidbody->Init();

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle(kPath);
	MV1SetScale(m_modelHandle, VECTOR(8.0f, 8.0f, 8.0f));

	//マップの当たり判定モデルのハンドルを取得
	//m_mapHandle = MapManager::GetInstance().GetStageCollisionHandle();

	//存在フラグをtrueにする
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

	//回転させる方向を計算する
	m_angle = -atan2f(m_moveDir.z, m_moveDir.x) - DX_PI_F / 2;
	m_rot = Vec3(0.0f, m_angle, 0.0f);

	MV1SetRotationXYZ(m_modelHandle, m_rot.ToVECTOR());
}

void Shot::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	m_frameCount++;

	auto dirNorm = m_moveDir.Normalize();
	rigidbody->SetVelocity(dirNorm * 4.0f);

	auto hitDim = MV1CollCheck_Sphere(m_mapHandle, -1, rigidbody->GetPosVECTOR(), kCollisionRadius);

	// 検出した周囲のポリゴン情報を開放する
	MV1CollResultPolyDimTerminate(hitDim);

	if (hitDim.HitNum != 0)
	{
		// 検出した周囲のポリゴン情報を開放する
		m_isExist = false;
	}

	if (m_frameCount > 300)
	{
		m_isExist = false;
	}
}

void Shot::Draw()
{
	//存在していない状態なら何もさせない
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