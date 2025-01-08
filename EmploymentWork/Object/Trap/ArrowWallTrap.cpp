#include "ArrowWallTrap.h"

#include "ResourceManager.h"
#include "LoadCSV.h"

namespace
{
	//当たり判定の円の半径
	constexpr float kCollisionRadius = 11.0f;

	//モデルサイズ
	constexpr float kModelScale = 1.8f;

	//ボーン（フレーム）の名前を指定
	const char* kTargetFrameName = "Obstacle_16_1";

}

ArrowWallTrap::ArrowWallTrap()
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;

	//罠のステータスを取得
	m_status = LoadCSV::GetInstance().LoadTrapStatus("ArrowWall");
}

ArrowWallTrap::~ArrowWallTrap()
{
}

void ArrowWallTrap::Init()
{
	//当たり判定を取るようにする
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	//存在フラグをtrueにする
	m_isExist = true;
}

void ArrowWallTrap::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;
}

void ArrowWallTrap::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;
}
