#include "PlayerStateJump.h"
#include "Input.h"
#include "Player.h"

#include "SoundManager.h"
#include "ResourceManager.h"
#include "LoadCSV.h"

//MEMO:ジャンプ中の状態遷移はStateで作るように変更する可能性あり

namespace
{
	//ジャンプ力
	constexpr float kJumpPower = 1.0f;
	//ジャンプフレーム数
	constexpr int kJumpFrame = 10;

	//上昇アニメーションの速度
	constexpr float kJumpUpAnimSpeed = 0.75f;	
	//下降アニメーションの速度
	constexpr float kJumpDownAnimSpeed = 0.7f;

	//上昇アニメーションの終了フレームのスケール
	constexpr float kJumpUpEndFrameScale = 0.98f;
	//下降アニメーションの終了フレームのスケール
	constexpr float kJumpDownEndFrameScale = 0.6f;	

	// 移動速度の調整係数
	constexpr float kVelocityOffset = 0.4f;
	// マップとの当たり判定のサイズ調整係数
	constexpr float kHitMapCollisionSizeScale = 2.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateJump::PlayerStateJump(std::shared_ptr<CharacterBase> own) :
	StateBase(own),
	m_jumpFrame(0)
{
	//現在のステートをダッシュ状態にする
	m_nowState = StateKind::Jump;

	own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx(own->GetCharacterName(), "JUMP_UP"), kJumpUpAnimSpeed);

	//このステートに入った瞬間にジャンプ力を足す
	auto vel = own->GetRigidbody()->GetVelocity();
	vel.y += kJumpPower;
	own->GetRigidbody()->SetVelocity(vel);

	//上昇状態にする
	m_updateFunc = &PlayerStateJump::UpUpdate;

	//ジャンプ音を流す
	SoundManager::GetInstance().PlaySE("S_PLAYERWALK");
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateJump::~PlayerStateJump()
{
}

/// <summary>
/// 初期化
/// </summary>
void PlayerStateJump::Init(std::string id)
{
	m_stageColId = id;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateJump::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckPlayer())	return;

	//メンバ関数ポインタの更新
	(this->*m_updateFunc)();
}

/// <summary>
/// ジャンプ上昇状態
/// </summary>
void PlayerStateJump::UpUpdate()
{
	//ジャンプフレームが上昇アニメーションの終了フレームに近づいたらジャンプ中状態にする
	if (m_jumpFrame >= m_pOwn.lock()->GetNowAnimEndFrame() * kJumpUpEndFrameScale)
	{
		//アニメーションを変える
		m_pOwn.lock()->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx("Player", "JUMP_AIR"));
		//ジャンプフレームを初期化する
		m_jumpFrame = 0;
		//ジャンプ中状態にする
		m_updateFunc = &PlayerStateJump::LoopUpdate;
	}

	//ジャンプフレームを更新する
	m_jumpFrame++;
}

/// <summary>
/// ジャンプ中状態
/// </summary>
void PlayerStateJump::LoopUpdate()
{
	//ジャンプフレームが規定数以上なら
	if (m_jumpFrame > kJumpFrame)
	{
		//ステージモデルを取得して設定
		auto stageModel = ResourceManager::GetInstance().GetHandle(m_stageColId);
		MV1SetScale(stageModel, VGet(0.01f, 0.01f, 0.01f));
		MV1SetRotationXYZ(stageModel, VGet(0.0f, DX_PI_F, 0.0f));

		//自身の足元座標と移動速度でカプセルを作る
		auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());
		auto pos = m_pOwn.lock()->GetRigidbody()->GetPos();
		auto vel = m_pOwn.lock()->GetRigidbody()->GetVelocity();
		auto modelBottomPos = pos;
		modelBottomPos.y -= own->GetCollisionSize();
		auto underPos = modelBottomPos;
		//カプセルの下側のY座標を移動速度に応じて下げる
		auto underPosYOffset = own->GetCollisionSize() * (kVelocityOffset - vel.y) * kHitMapCollisionSizeScale;
		underPos.y -= own->GetCollisionRadius() + underPosYOffset;

		//作ったカプセルとステージモデルで当たり判定をとる
		auto hit = MV1CollCheck_Line(stageModel, -1, modelBottomPos.ToVECTOR(), underPos.ToVECTOR());

		//ステージとカプセルが当たっていたらジャンプ下降状態にする
		if (hit.HitFlag)
		{
			//アニメーションを変える
			own->ChangeAnim(LoadCSV::GetInstance().GetAnimIdx("Player", "JUMP_DOWN"), kJumpDownAnimSpeed);
			//ジャンプフレームを初期化する
			m_jumpFrame = 0;
			//ジャンプ下降状態にする
			m_updateFunc = &PlayerStateJump::DownUpdate;
		}

#ifdef _DEBUG	//デバッグ描画
		//カプセルの描画
		DrawCapsule3D(modelBottomPos.ToVECTOR(), underPos.ToVECTOR(), 2, 5, 0xcccccc, 0xcccccc, false);
#endif
		//ステージモデルの削除
		MV1DeleteModel(stageModel);

	}

	//ジャンプフレームを更新する
	m_jumpFrame++;
}

/// <summary>
/// ジャンプ下降状態
/// </summary>
void PlayerStateJump::DownUpdate()
{
	//ダウンキャスト
	auto own = std::dynamic_pointer_cast<Player>(m_pOwn.lock());

	//ジャンプフレームが上昇アニメーションの終了フレーム以上なら入力に応じてステートを変更する
	if (m_jumpFrame >= m_pOwn.lock()->GetNowAnimEndFrame() * kJumpDownEndFrameScale)
	{
		//ジャンプ音を流す
		SoundManager::GetInstance().PlaySE("S_PLAYERWALK");

		//左スティックが入力されていなかったらStateをIdleにする
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			ChangeState(StateKind::Idle);
			return;
		}

		//左スティックが入力されていたらStateをWalkにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			ChangeState(StateKind::Walk);
			return;
		}
	}

	//ジャンプフレームを更新する
	m_jumpFrame++;
}
