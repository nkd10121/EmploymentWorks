#include "SpikeTrap.h"

#include "ModelManager.h"
#include "ResourceManager.h"
#include "LoadCSV.h"

namespace
{
	//当たり判定の円の半径
	constexpr float kCollisionRadius = 11.0f;

	//モデルサイズ
	//constexpr float kModelScale = 0.08f;
	constexpr float kModelScale = 1.8f;

	constexpr float kSpikeMoveSpeed = 0.8f;

	//スパイクモデルが動く幅。大きくすればするほど大きく動く。
	constexpr float kSpikeModelMoveRange = 140.0f;
	//サインカーブの制限。(0.0f～0.1f)
	constexpr float kSinLimit = 0.08f;

	const char* kTargetFrameName = "wooden_spikes.001"; // ボーン（フレーム）の名前を指定

}

SpikeTrap::SpikeTrap() :
	TrapBase(),
	m_spikeModel(-1),
	m_attackCount(0),
	m_spikePos(),
	m_norm(),
	m_movedPos()
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;

	m_status = LoadCSV::GetInstance().LoadTrapStatus("Spike");
}


SpikeTrap::~SpikeTrap()
{
	MV1DeleteModel(m_spikeModel);
}


void SpikeTrap::Init(Vec3 pos, Vec3 norm)
{
	OnEntryPhysics();

	//物理挙動の初期化
	rigidbody->Init();

	rigidbody->SetPos(pos);
	rigidbody->SetNextPos(pos);

	m_spikePos = pos;
	m_spikePos -= norm * 2.6f;
	m_spikePosInit = m_spikePos;

	//モデルのハンドルを取得
	//m_modelHandle = ResourceManager::GetInstance().GetHandle("M_SPIKEFRAME");
	//MV1SetScale(m_modelHandle, VECTOR(kModelScale, kModelScale, kModelScale));
	//MV1SetPosition(m_modelHandle, pos.ToVECTOR());
	m_spikeModel = ResourceManager::GetInstance().GetHandle("M_TEMP_SPIKE");
	MV1SetScale(m_spikeModel, VECTOR(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());

	//第二引数の法線ベクトルに沿ってモデルの向きを回転させたい
	m_norm = norm;

	//MV1SetRotationXYZ(m_modelHandle, VGet(norm.z * (DX_PI_F / 2), 0.0f, -norm.x * (DX_PI_F / 2)));
	MV1SetRotationXYZ(m_spikeModel, VGet(norm.z * (DX_PI_F / 2), 0.0f, -norm.x * (DX_PI_F / 2)));

	m_frameIdx = MV1SearchFrame(m_spikeModel, kTargetFrameName);

	auto mat = MV1GetFrameLocalWorldMatrix(m_spikeModel, m_frameIdx);
	mat.m[3][0] = m_spikePosInit.x;
	mat.m[3][1] = m_spikePosInit.y;
	mat.m[3][2] = m_spikePosInit.z;

	MV1SetFrameUserLocalWorldMatrix(m_spikeModel, m_frameIdx, mat);

	//存在フラグをtrueにする
	m_isExist = true;
}


void SpikeTrap::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	if (m_isAttack)
	{
		//攻撃用当たり判定を生成する
		if (m_attackCount == 0)
		{
			//当たり判定の生成
			auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Attack);
			auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
			sphereCol->m_radius = kCollisionRadius;
		}

		//攻撃カウントを更新
		m_attackCount++;

		//現在の攻撃フレームと1フレーム前のsinカーブを計算する
		auto presin = sinf(DX_PI_F * (m_attackCount - 1) * 0.0125f) * 0.4f;
		auto sin = sinf(DX_PI_F * m_attackCount * 0.0125f) * 0.4f;

		//二つのsinカーブを比較して上昇か下降かを計算する
		auto move = (sin - presin) * kSpikeModelMoveRange;



		//制限より小さいときはモデルを動かす
		if (sin < kSinLimit)
		{
			auto mat = MV1GetFrameLocalWorldMatrix(m_spikeModel, m_frameIdx);
			mat.m[3][0] += (m_norm * move).x;
			mat.m[3][1] += (m_norm * move).y;
			mat.m[3][2] += (m_norm * move).z;

			MV1SetFrameUserLocalWorldMatrix(m_spikeModel, m_frameIdx, mat);

			//m_spikePos += m_norm * move;
			//MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());
		}

		//サインカーブが0以下になった時に攻撃中から抜け出す
		if (sin < 0.0f)
		{
			m_isAttack = false;

			auto mat = MV1GetFrameLocalWorldMatrix(m_spikeModel, m_frameIdx);
			mat.m[3][0] = m_spikePosInit.x;
			mat.m[3][1] = m_spikePosInit.y;
			mat.m[3][2] = m_spikePosInit.z;

			MV1SetFrameUserLocalWorldMatrix(m_spikeModel, m_frameIdx, mat);

			m_spikePos = m_spikePosInit;
			m_movedPos = Vec3();
			MV1SetPosition(m_spikeModel, m_spikePos.ToVECTOR());
		}
	}
	else
	{
		m_attackCount = 0;
	}
}


void SpikeTrap::Draw()
{
	//MV1DrawModel(m_modelHandle);
	MV1DrawModel(m_spikeModel);
}