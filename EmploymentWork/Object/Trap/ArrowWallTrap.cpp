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


	//罠のステータスを取得
	m_status = LoadCSV::GetInstance().LoadTrapStatus("ArrowWall");


}

ArrowWallTrap::~ArrowWallTrap()
{
}

void ArrowWallTrap::Init(Vec3 pos,Vec3 norm)
{
	//当たり判定を取るようにする
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	//座標の更新
	rigidbody->SetPos(pos);
	rigidbody->SetNextPos(pos);

	//モデルのハンドルを取得
	m_modelHandle = ResourceManager::GetInstance().GetHandle("M_ARROWWALL");
	MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	//回転させる
	//atan2を使用して向いている角度を取得
	auto angle = atan2(norm.x, norm.z);
	auto rotation = VGet(0.0f, angle + DX_PI_F, 0.0f);
	MV1SetRotationXYZ(m_modelHandle, rotation);

	//索敵判定の作成(3つ作成)
	for (int i = 1; i < 4; i++)
	{
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
		auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
		sphereCol->m_radius = kCollisionRadius;

		auto searchPos = norm * i * 20.0f;
		sphereCol->SetOffsetPos(searchPos);
	}

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

	//モデルの描画
	MV1DrawModel(m_modelHandle);
}
