#include "EnemyBase.h"
#include "TrapBase.h"
#include "Player.h"
#include "Shot.h"

#include "EnemyStateWalk.h"
#include "EnemyStateDeath.h"
#include "EnemyStateDamaged.h"

#include "LoadCSV.h"
#include "EffectManager.h"
#include "SoundManager.h"
namespace
{
	//攻撃判定の半径
	constexpr float kAttackCollisionRadius = 4.0f;
	constexpr float kAttackCollisionDirection = 4.0f;

	constexpr int kRandMax = 12;

	//攻撃してきたオブジェクト名配列をリセットするまでの時間
	constexpr int kAttackerNameClearLimit = 60 * 6;

	std::map<std::string, float> kModelSize =
	{
		{"EnemyNormal", 0.0007f },
		{"EnemyFast", 0.00055f },
		{"EnemyBig", 0.001f },
	};

	const char* kAttachFrameName = "Bip001 Head";

}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyBase::EnemyBase() :
	CharacterBase(Collidable::Priority::High, GameObjectTag::Enemy),
	kind(),
	m_isExist(false),
	m_isStartDeathAnimation(false),
	m_routeIdx(0),
	m_collisionRadius(0.0f),
	m_collisionSize(0.0f),
	m_drawPos(),
	m_isSearchInPlayer(false),
	m_hitObjectTag(),
	m_lastAttackTag(),
	m_isOffensive(true),
	m_isAttack(true),
	m_attackerNameClearCount(0),
	m_attackerNameClearLimit(kAttackerNameClearLimit),
	m_moveDebuff(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
	//モデルを削除する
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
void EnemyBase::Init()
{
	//スタートウェイポイントの座標に設定する
	SetPos(m_route[0].pos);

	//当たり判定の初期化
	OnEntryPhysics();

	//ステートパターンの初期化
	m_pState = std::make_shared<EnemyStateWalk>(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
	m_pState->SetNextKind(StateBase::StateKind::Walk);
	m_pState->Init("");

	//物理データの初期化
	rigidbody->Init(true);
	rigidbody->SetPos(m_drawPos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//モデルのサイズを変更
	MV1SetScale(m_modelHandle, VGet(kModelSize[m_characterName], kModelSize[m_characterName], kModelSize[m_characterName]));
	MV1SetPosition(m_modelHandle, m_drawPos.ToVECTOR());

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, LoadCSV::GetInstance().GetAnimIdx(m_characterName, "WALK_FORWARD"));
	m_preAnimIdx = 0;
	m_nowAnimIdx = 0;

	//ステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus(m_characterName.c_str());
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	//当たり判定をとらないオブジェクトタグを設定
	AddThroughTag(GameObjectTag::Portion);		//ポーション
	AddThroughTag(GameObjectTag::SwarmEnemy);	//敵群れ

	{
		//通常の当たり判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);	//追加
		auto capsuleCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());			//キャスト
		capsuleCol->m_radius = m_collisionRadius;		//カプセルの半径
		capsuleCol->m_size = m_collisionSize;			//カプセルの大きさ

	}

	{
		//ヘッドショット判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Head);	//追加
		auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());			//キャスト
		sphereCol->m_radius = 2.0f;		//カプセルの半径

		auto attachFrameNum = MV1SearchFrame(m_modelHandle, kAttachFrameName);
		auto mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, attachFrameNum);
		auto pos = Vec3(mat.m[3][0], mat.m[3][1], mat.m[3][2]);

		auto modelCenterPos = rigidbody->GetPos();

		auto vec = pos - modelCenterPos;
		vec.y *= 0.55f;

		sphereCol->SetOffsetPos(vec);
	}

	//存在フラグをtrueにする
	m_isExist = true;
}

/// <summary>
/// 終了
/// </summary>
void EnemyBase::Finalize()
{
	if (m_isExist)
	{
		//当たり判定の削除
		Collidable::OnExistPhysics();
	}
}

/// <summary>
/// 更新
/// </summary>
void EnemyBase::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//攻撃してきたオブジェクト名配列のサイズが0以外なら
	if (m_attackerName.size() != 0)
	{
		//条件にあっていたら
		if (m_attackerNameClearCount > m_attackerNameClearLimit)
		{
			//配列を削除する
			m_attackerName.clear();

			//カウントを初期化する
			m_attackerNameClearCount = 0;
		}
		else
		{
			//カウントを更新する
			m_attackerNameClearCount++;
		}
	}

	//ステートの更新
	m_pState->Update();

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);
	//アニメーションブレンド
	AnimationBlend();

	//HPが0になったら自身を削除する
	if (m_status.hp <= 0 && !m_isStartDeathAnimation)
	{
		//当たり判定を削除
		Finalize();

		//死亡アニメーションを開始する
		m_isStartDeathAnimation = true;

		//現在のステートを強制的に死亡にする
		m_pState = std::make_shared<EnemyStateDeath>(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
		m_pState->SetNextKind(StateBase::StateKind::Death);
		m_pState->Init("");
	}

	//死亡アニメーションが始まっていて
	if (m_isStartDeathAnimation)
	{
		//死亡アニメーションが終了したら
		if (GetAnimEnd())
		{
			//完全に死亡したものとする
			m_isExist = false;

			EffectManager::GetInstance().CreateEffect("E_ENEMYDEAD", rigidbody->GetPos());
		}
	}

	m_moveDebuff = 1.0f;

#ifdef _DEBUG
	//何の当たり判定を持っているかをデバッグ描画
	for (auto& col : m_colliders)
	{
		switch (col->collideTag)
		{
		case MyLib::ColliderBase::CollisionTag::Normal:
			printf("通常の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Attack:
			printf("攻撃の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Search:
			printf("索敵の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Head:
			printf("頭の当たり判定\n");
			break;
		default:
			break;
		}
	}
#endif
}

/// <summary>
/// 描画
/// </summary>
void EnemyBase::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//モデルを描画
	MV1DrawModel(m_modelHandle);
}

/// <summary>
/// 移動ルートを設定する
/// </summary>
void EnemyBase::SetRoute(const std::vector<EnemyManager::WayPoint> wayPoints)
{
	for (auto& wp : wayPoints)
	{
		auto add = wp;
		int rand = GetRand(kRandMax) - kRandMax/2;
		add.pos.x += rand;
		add.pos.y += 6.0f;
		rand = GetRand(kRandMax) - kRandMax/2;
		add.pos.z += rand;
		m_route.push_back(add);
	}
}

/// <summary>
/// 座標を設定
/// </summary>
void EnemyBase::SetPos(Vec3 pos)
{
	m_drawPos = pos;
}

/// <summary>
/// ほかのオブジェクトと衝突したときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	bool isDamaged = false;

	if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Normal)
	{
		//当たったオブジェクトがプレイヤーが撃った弾なら
		if (m_hitObjectTag == GameObjectTag::PlayerShot)
		{
			isDamaged = true;
			//弾の攻撃力分自身のHPを減らす(防御力と調整しながら)
			Shot* col = dynamic_cast<Shot*>(send.get());
			auto damage = col->GetAtk() - m_status.def;
			if (damage > 0)
			{
				m_status.hp -= damage;
			}
			//攻撃してきたタグを保存
			m_lastAttackTag = m_hitObjectTag;
			//敵ヒットSEを流す
			SoundManager::GetInstance().PlaySE("S_ENEMYHIT");
			//敵ヒットエフェクトを出す
			EffectManager::GetInstance().CreateEffect("E_ENEMYHIT", rigidbody->GetPos());
			//当たった弾の終了処理を呼ぶ
			col->End();

			AddAttackerName("Player");
		}
		//当たったオブジェクトがトラップなら
		else if (m_hitObjectTag == GameObjectTag::Trap)
		{
			if (sendCol->collideTag == MyLib::ColliderBase::CollisionTag::Attack)
			{
				isDamaged = true;
				//そのトラップの攻撃力分HPを減らす
				TrapBase* col = dynamic_cast<TrapBase*>(send.get());
				auto damage = col->GetAtk() - m_status.def;
				if (damage > 0)
				{
					m_status.hp -= damage;
				}

				//攻撃してきたタグを保存
				m_lastAttackTag = m_hitObjectTag;

				EffectManager::GetInstance().CreateEffect("E_TRAPATTACK", rigidbody->GetPos());

				AddAttackerName(col->GetTrapName());
			}
		}
	}
	else if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
	{
		//当たったオブジェクトがプレイヤーがなら
		if (m_hitObjectTag == GameObjectTag::Player)
		{
			m_isSearchInPlayer = true;
			if (m_pState->GetKind() != StateBase::StateKind::Attack)
			{
				m_pState->SetNextKind(StateBase::StateKind::Walk);
			}
		}
	}
	else if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Head)
	{
		//当たったオブジェクトがプレイヤーが撃った弾なら
		if (m_hitObjectTag == GameObjectTag::PlayerShot)
		{
			{
				isDamaged = true;
				//弾の攻撃力分自身のHPを減らす(防御力と調整しながら)
				Shot* col = dynamic_cast<Shot*>(send.get());
				auto damage = col->GetAtk() - m_status.def;
				if (damage > 0)
				{
					//通常の3倍のダメージを与える
					m_status.hp -= damage * 3;
				}
				//攻撃してきたタグを保存
				m_lastAttackTag = m_hitObjectTag;
				//敵ヒットSEを流す
				SoundManager::GetInstance().PlaySE("S_ENEMYHIT");
				//敵ヒットエフェクトを出す
				EffectManager::GetInstance().CreateEffect("E_ENEMYCRITICALHIT", rigidbody->GetPos());
				//当たった弾の終了処理を呼ぶ
				col->End();

				AddAttackerName("Player");
			}
		}
	}

	//ダメージを食らっていたら
	if (isDamaged)
	{
		//30%の確率で被ダメージ状態にする
		if (GetRand(99) + 1 <= 30)
		{
			m_pState = std::make_shared<EnemyStateDamaged>(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
			m_pState->SetNextKind(StateBase::StateKind::Damaged);
			m_pState->Init("");
		}
	}
}

/// <summary>
/// ほかのオブジェクトと衝突しているときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerStay(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Normal)
	{
		if (m_hitObjectTag == GameObjectTag::Trap)
		{
			auto trap = dynamic_cast<TrapBase*>(send.get());
			if (trap->GetTrapName() == "IronSnare")
			{
				if (sendCol->collideTag == MyLib::ColliderBase::CollisionTag::Attack)
				{
					m_moveDebuff = 0.4f;
				}
			}

			AddAttackerName(trap->GetTrapName());
		}
	}

	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	//当たったオブジェクトがプレイヤーのとき
	if (m_hitObjectTag == GameObjectTag::Player)
	{
		//当たったコリジョンが索敵の時
		if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
		{
			if (m_isSearchInPlayer)
			{
				Player* col = dynamic_cast<Player*>(send.get());
				m_playerPos = col->GetRigidbody()->GetPos();
			}
		}
	}
}

/// <summary>
/// ほかのオブジェクトと衝突しなくなったときに呼ばれる
/// </summary>
void EnemyBase::OnTriggerExit(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	//当たったオブジェクトがプレイヤーのとき
	if (m_hitObjectTag == GameObjectTag::Player)
	{
		//当たったコリジョンが索敵の時
		if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
		{
			m_isSearchInPlayer = false;
			if (m_pState->GetKind() != StateBase::StateKind::Attack)
			{
				m_pState->SetNextKind(StateBase::StateKind::Idle);
			}
		}
	}
}

/// <summary>
/// 3Dモデルの座標更新
/// </summary>
void EnemyBase::UpdateModelPos()
{
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_drawPos = rigidbody->GetPos();
	m_drawPos.y -= m_collisionRadius + m_collisionSize;
	MV1SetPosition(m_modelHandle, m_drawPos.ToVECTOR());
}

/// <summary>
/// ドロップする罠ポイントを取得
/// </summary>
const int EnemyBase::GetDropPoint() const
{
	//攻撃してきたオブジェクト名の配列のサイズが0か1なら設定されていたポイントをそのまま返す
	if (m_attackerName.size() == 0 || m_attackerName.size() == 1)
	{
		//auto screenPos = ConvWorldPosToScreenPos(rigidbody->GetPosVECTOR());
		//DrawFormatString(screenPos.x,screenPos.y, 0xff0000,"%d", m_status.point);

		return m_status.point;
	}
	//サイズが2以上だったらボーナスポイントを足して返す
	else
	{
		float thirtyPer = static_cast<float>(m_status.point) * 0.3f;

		//auto screenPos = ConvWorldPosToScreenPos(rigidbody->GetPosVECTOR());
		//DrawFormatString(screenPos.x, screenPos.y, 0xff0000, "%d", m_status.point + static_cast<int>(twentyPer) * static_cast<int>(m_attackerName.size()));

		return m_status.point + static_cast<int>(thirtyPer) * static_cast<int>(m_attackerName.size());
	}
}

/// <summary>
/// 索敵判定の半径を取得
/// </summary>
const float EnemyBase::GetSearchCollisionRadius() const
{
	return  m_collisionRadius * 10;
}

/// <summary>
/// モデルの向きを設定
/// </summary>
const void EnemyBase::SetModelRotation(Vec3 rot) const
{
	MV1SetRotationXYZ(m_modelHandle, rot.ToVECTOR());

}

/// <summary>
/// ヘッドショット判定の向きを設定
/// </summary>
const void EnemyBase::SetHeadCollisionFrontVec(Vec3 vec) const
{
	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Head);
	if (col != nullptr)
	{
		col->collide->localPos.SetFrontPos(vec);
	}
}

/// <summary>
/// 索敵判定を生成する
/// </summary>
void EnemyBase::CreateSearchCollision()
{
	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = m_collisionRadius * 10;
}

/// <summary>
/// 索敵判定を削除する
/// </summary>
void EnemyBase::DeleteSearchCollision()
{
	Collidable::DeleteRequestCollider(Collidable::GetCollider(MyLib::ColliderBase::CollisionTag::Search));
}

/// <summary>
/// 攻撃判定を作成
/// </summary>
const void EnemyBase::CreateAttackCollision(Vec3 vec)
{
	m_isAttack = true;

	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Attack);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kAttackCollisionRadius;

	auto attackPos = vec.Normalize() * kAttackCollisionDirection;
	attackPos = Vec3(attackPos.x, 0.0f, attackPos.z);

	sphereCol->localPos.localPos = attackPos;

	return void();
}

/// <summary>
/// 攻撃判定を削除
/// </summary>
const void EnemyBase::DeleteAttackCollision()
{
	m_isAttack = false;

	auto col = GetCollider(MyLib::ColliderBase::CollisionTag::Attack);
	if (col == nullptr)return;		//もし攻撃の判定を持っていなかったら早期リターン

	Collidable::DeleteRequestCollider(col);

	return void();
}

/// <summary>
/// 指定した当たり判定タグが存在するかチェック
/// </summary>
const bool EnemyBase::CheckIsExistCollisionTag(MyLib::ColliderBase::CollisionTag tag) const
{
	//存在するならtrue,しなかったらfalse
	for (auto& col : m_colliders)
	{
		if (col->collideTag == tag)
		{
			return true;
		}
	}
	return false;
}

const void EnemyBase::AddAttackerName(std::string name)
{
	for (auto& attackerName : m_attackerName)
	{
		if (attackerName == name)
		{
			return;
		}
	}

	m_attackerName.push_back(name);
}
