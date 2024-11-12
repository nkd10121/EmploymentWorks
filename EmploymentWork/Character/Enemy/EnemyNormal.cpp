#include "EnemyNormal.h"
#include "EnemyStateIdle.h"

#include "LoadCSV.h"
#include "ModelManager.h"

namespace
{
	/*当たり判定用のカプセル関係*/
	constexpr float kCollisionCapsuleSize = 3.0f;	//カプセルの大きさ
	constexpr float kCollisionCapsuleRadius = 2.0f;	//カプセルの半径
#ifdef _DEBUG	//デバッグ描画
	constexpr int kCollisionCapsuleDivNum = 10;		//カプセルの分割数
#endif

	/*モデル関係*/
	constexpr float kModelScale = 0.018f;		//モデルサイズ
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyNormal::EnemyNormal():
	EnemyBase()
{
	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCollisionCapsuleRadius;		
	sphereCol->m_size = kCollisionCapsuleSize;

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_ENEMYNORMAL");
	//モデルのサイズを変更
	MV1SetScale(m_modelHandle,VGet(kModelScale, kModelScale, kModelScale));
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyNormal::~EnemyNormal()
{
}

/// <summary>
/// 初期化
/// </summary>
void EnemyNormal::Init(std::shared_ptr<MyLib::Physics> physics)
{
	//物理クラスを取得
	m_pPhysics = physics;

	//当たり判定の初期化
	Collidable::Init(physics);

	//ステートパターンの初期化
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
	m_pState->SetNextState(m_pState);
	auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_pState);
	state->Init();

	//座標を仮決定
	//TODO:モデルの座標を設定する部分が作れたら削除する
	m_drawPos.x = static_cast<float>(GetRand(20) - 10);
	m_drawPos.y = 8.0f;
	m_drawPos.z = static_cast<float>(GetRand(20) - 10);

	//物理データの初期化
	rigidbody->Init(true);
	rigidbody->SetPos(m_drawPos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus("EnemyNormal");
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	//存在フラグをtrueにする
	m_isExist = true;
}

/// <summary>
/// 終了
/// </summary>
void EnemyNormal::Finalize()
{
	//当たり判定の削除
	Collidable::Finalize(m_pPhysics.lock());
}

/// <summary>
/// 更新
/// </summary>
void EnemyNormal::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->GetNextState()->GetKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->GetNextState();
		m_pState->SetNextState(m_pState);
	}

	//ステートの更新
	m_pState->Update();

	//HPが0になったら自身を削除する
	if (m_status.hp <= 0)
	{
		m_isExist = false;
	}

	//モデルの描画座標を設定
	auto modelSetPos = m_drawPos;
	modelSetPos.y -= kCollisionCapsuleSize + kCollisionCapsuleRadius;
	MV1SetPosition(m_modelHandle, modelSetPos.ToVECTOR());

	//速度を0にする(重力の影響を受けながら)
	auto prevVel = rigidbody->GetVelocity();
	rigidbody->SetVelocity(Vec3(0.0f, prevVel.y, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void EnemyNormal::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//FIX:Drawのなかで座標を変更しているのはどうなの？
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_drawPos = rigidbody->GetPos();

	//プレイヤー想定のカプセル
	VECTOR low = VGet(m_drawPos.x, m_drawPos.y - kCollisionCapsuleSize, m_drawPos.z);
	VECTOR high = VGet(m_drawPos.x, m_drawPos.y + kCollisionCapsuleSize, m_drawPos.z);
	DrawCapsule3D(low, high, kCollisionCapsuleRadius, kCollisionCapsuleDivNum, 0xffffff, 0xffffff, false);

	//モデルを描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG	//デバッグ描画
	//ステートパターンの確認
	m_pState->DebugDrawState(128, 32);
#endif
}

