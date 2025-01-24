#include "FlameTrap.h"

#include "LoadCSV.h"
#include "ResourceManager.h"

FlameTrap::FlameTrap() :
	TrapBase()
{
	m_trapName = "Frame";
	//罠のステータスを取得
	m_status = LoadCSV::GetInstance().LoadTrapStatus(m_trapName.c_str());
}

FlameTrap::~FlameTrap()
{
}

void FlameTrap::Init(Vec3 pos, Vec3 direction)
{
	//当たり判定を取るようにする
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	//座標の更新
	rigidbody->SetPos(pos);
	rigidbody->SetNextPos(pos);

	//設置場所に座標を移動させる
	m_direction = direction;

	//モデルのハンドルを取得
	m_modelHandle = ResourceManager::GetInstance().GetHandle("M_FLAME");
	MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	MV1SetScale(m_modelHandle, Vec3(16.0f, 16.0f, 16.0f).ToVECTOR());
	//回転させる
	//atan2を使用して向いている角度を取得
	auto angle = atan2(direction.x, direction.z);
	auto rotation = VGet(0.0f, angle + DX_PI_F, 0.0f);
	MV1SetRotationXYZ(m_modelHandle, rotation);

	//索敵判定の作成(3つ作成)
	for (int i = 1; i < 4; i++)
	{
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
		auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
		sphereCol->m_radius = m_status.searchRange;

		auto searchPos = direction * static_cast<float>(i * m_status.searchRange * 2);
		sphereCol->SetOffsetPos(searchPos);

		//索敵判定は動かすつもりがないため、先に中心座標を設定して動かないようにする
		sphereCol->SetCenterPos(pos);
		sphereCol->UseIsStatic();
	}

	//存在フラグをtrueにする
	m_isExist = true;
}

void FlameTrap::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;
}

void FlameTrap::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//モデルの描画
	MV1DrawModel(m_modelHandle);
}
