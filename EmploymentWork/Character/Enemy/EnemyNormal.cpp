#include "EnemyNormal.h"
#include "EnemyStateIdle.h"
#include "EnemyStateDeath.h"

#include "LoadCSV.h"
#include "ModelManager.h"

namespace
{
	/*当たり判定用のカプセル関係*/
	constexpr float kCollisionCapsuleSize = 3.0f;	//カプセルの大きさ
	constexpr float kCollisionCapsuleRadius = 2.0f;	//カプセルの半径
	constexpr int kCollisionCapsuleDivNum = 10;		//カプセルの分割数
#ifdef _DEBUG	//デバッグ描画
#endif

	/*モデル関係*/
	constexpr float kModelScale = 0.018f;		//モデルサイズ

	/*アニメーション関係*/
	constexpr float kAnimChangeFrame = 10.0f;							//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;		//1フレーム当たりのアニメーション切り替えが進む速さ
	constexpr float kAnimBlendRateMax = 1.0f;							//アニメーションブレンド率の最大

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

	//キャラクター名を設定
	m_characterName = "EnemyNormal";
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
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<EnemyNormal>(shared_from_this()));
	m_pState->SetNextKind(StateBase::StateKind::Idle);
	m_pState->Init();

	//物理データの初期化
	rigidbody->Init(true);
	rigidbody->SetPos(m_drawPos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_ENEMYNORMAL");
	//モデルのサイズを変更
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, LoadCSV::GetInstance().GetAnimIdx(m_characterName, "IDLE"));
	m_preAnimIdx = 0;
	m_nowAnimIdx = 0;

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
	if (m_pState->GetNextKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->GetNextScenePointer();
	}

	//ステートの更新
	m_pState->Update();

	//アニメーションが終了したかどうかを取得
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);

	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendRateMax)
		{
			m_animBlendRate = kAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, kAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	}

	//HPが0になったら自身を削除する
	if (m_status.hp <= 0 && !m_isStartDeathAnimation)
	{
		Finalize();

		m_isStartDeathAnimation = true;

		m_pState = std::make_shared<EnemyStateDeath>(std::dynamic_pointer_cast<EnemyNormal>(shared_from_this()));
		m_pState->SetNextKind(StateBase::StateKind::Death);
		m_pState->Init();
	}

	if (m_isStartDeathAnimation)
	{
		if (GetAnimEnd())
		{
			m_isExist = false;
		}
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

	//モデルを描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG	//デバッグ描画
	//ステートパターンの確認
	m_pState->DebugDrawState(128, 32);
#endif
}

