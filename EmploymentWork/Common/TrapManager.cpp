#include "TrapManager.h"
#include "MathHelp.h"

#include "SpikeTrap.h"

#include "ImageManager.h"
#include "Input.h"
#include "ResourceManager.h"
#include "LoadCSV.h"

TrapManager* TrapManager::m_instance = nullptr;

namespace
{
	const std::string kStageDataPathFront = "data/stageData/";
	const std::string kStageDataPathBack = ".tLoc";
}

TrapManager::TrapManager():
	m_previewTrapModelHandle(-1),
	m_angle(0.0f),
	m_transparency(0.0f),
	m_slotIdx(-1),
	m_cameraPos(),
	m_cameraDir(),
	m_trapPoint(0),
	m_rightTriggerPushCount(0),
	m_bgHandle(-1)
{
	m_trapModelHandles.push_back(std::make_pair(ResourceManager::GetInstance().GetHandle("M_SPIKE"), 1.8f));
}

TrapManager::~TrapManager()
{
	Clear();
}

const bool TrapManager::CheckNeighbor(std::list<std::weak_ptr<Trap>> check) const
{
	for (auto& t : check)
	{
		if (t.lock()->isPlaced)	return false;
	}
	return true;
}

void TrapManager::AddTrapPos(Vec3 pos)
{
	//std::shared_ptr<Trap> add = std::make_shared<Trap>();
	//add->isPlaced = false;
	//add->pos = pos;
	//add->neighborTraps.clear();
	//m_trapPoss.emplace_back(add);
}

void TrapManager::Update()
{
	//現在設置しているトラップの更新
	for (auto& trap : m_traps)
	{
		trap->Update();
	}

	//現在プレイヤーが選択しているスロット番号、カメラの座標と向きベクトルを事前にもらっておく

	//スロット番号が0(クロスボウなら何もしない)
	if (m_slotIdx == 0) return;

	m_angle += 0.04f;
	m_transparency = abs(sinf(m_angle) / 2.5f) + 0.1f;

	//それ以外なら罠を設置しようとしている

	auto start = m_cameraPos;					//カメラ座標を開始座標
	auto end = m_cameraPos + m_cameraDir * 60;	//カメラからカメラの向いている方向の座標を終点座標にする

	//上の二つをつなぐ線分と罠の座標の距離を格納する変数
	float defaultLength = 100.0f;

	//MEMO:今までしていた四角で先に判定を取る方法をいったん消しているが露骨に処理時間が増えてそう

	//設置可能なトラップの座標分回す
	for (auto& trapPos : m_trapPoss)
	{
		//トラップが置かれていないかつ、周囲に8個のトラップがおかれていない候補地があるとき
		if (!trapPos->isPlaced && trapPos->neighborTraps.size() == 8 && CheckNeighbor(trapPos->neighborTraps))
		{
			//もし、設置しようとしているトラップの種類がトラップ座標候補の法線ベクトルと一致していなければ次に行く
			//罠ごとに設定されている種類を取得できるようにしたい
			auto data = 0;
			if (data == 0)
			{
				if (abs(trapPos->norm.y - 1.0f) > 0.1f)
				{
					continue;
				}
			}
			else if (data == 1)
			{
				if(abs(trapPos->norm.y) > 0.1f)
				{
					continue;
				}
			}

			//線分と座標の距離を計算する
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trapPos->pos.ToVECTOR());

			//もし、上で計算した距離が今までの距離より短かったらその距離と候補地を保存する
			if (defaultLength > length)
			{
				defaultLength = length;
				debugTrap = trapPos;
			}
		}
	}

	//もしライトトリガーボタン(RT)が押されたら
	if (Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		//トリガーボタンを押した瞬間なら
		if (m_rightTriggerPushCount == 0)
		{
			switch (m_slotIdx)
			{
			case 1:
				{
					auto add = std::make_shared<SpikeTrap>();
					//もし設置しようとしていたトラップのコストよりも現在持っているポイントが少なかったら設置できない
					if (m_trapPoint < add->GetCost())
					{
						//何もしない
						return;
					}

					//所持トラップポイントをコスト分減らす
					m_trapPoint -= add->GetCost();

					//初期化
					add->Init(debugTrap->pos, debugTrap->norm);

					//追加
					m_traps.emplace_back(add);

					//トラップを設置済みにする
					debugTrap->isPlaced = true;
					for (auto& trap : debugTrap->neighborTraps)
					{
						trap.lock()->isPlaced = true;
					}
				}
				break;
			default:
				break;
			}
		}

		//トリガーボタンを押したカウントを更新する
		m_rightTriggerPushCount++;
	}
	else
	{
		m_rightTriggerPushCount = 0;
	}
}

void TrapManager::Draw()
{
	for (auto& trap : m_traps)
	{
		trap->Draw();
	}



#ifdef _DEBUG	//デバッグ描画
	for (auto& pos : m_trapPoss)
	{
		if (pos->isPlaced)
		{
			DrawSphere3D(pos->pos.ToVECTOR(), 3, 4, 0xffffff, 0xffffff, false);
		}
		else
		{
			DrawSphere3D(pos->pos.ToVECTOR(), 3, 4, 0xffff00, 0xffff00, false);
		}
	}

	if (debugTrap != nullptr)
	{
		if(!debugTrap->isPlaced && debugTrap->neighborTraps.size() == 8 && CheckNeighbor(debugTrap->neighborTraps))
		{
			DrawSphere3D(debugTrap->pos.ToVECTOR(), 4, 4, 0x00ff00, 0x00ff00, false);
		}
		else
		{
			DrawSphere3D(debugTrap->pos.ToVECTOR(), 4, 4, 0xff0000, 0xff0000, false);
		}
	}
#endif

	DrawRotaGraph(80, 660, 0.72f, 0.0f, m_bgHandle, true);
	DrawFormatString(64, 720 - 16 * 4, 0xffffff, "%d", m_trapPoint);
}

void TrapManager::PreviewDraw()
{
	//仮設置描画をする
	//現在のスロット番号から対応する罠のハンドルを取得して半透明描画する
	//	罠のサイズがそれぞれ異なるため、どうにかして設定するためにスケール値を取得する必要がある

	if (m_slotIdx == 0) return;

	m_previewTrapModelHandle = m_trapModelHandles[m_slotIdx - 1].first;
	auto scale = m_trapModelHandles[m_slotIdx - 1].second;
	MV1SetScale(m_previewTrapModelHandle, VGet(scale, scale, scale));
	MV1SetPosition(m_previewTrapModelHandle, debugTrap->pos.ToVECTOR());
	MV1SetOpacityRate(m_previewTrapModelHandle, m_transparency);

	//TODO:半透明描画が多分これだとできない？調べる必要あり
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	MV1DrawModel(m_previewTrapModelHandle);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void TrapManager::Load(const char* stageName)
{
	//開くファイルのハンドルを取得
	int handle = FileRead_open((kStageDataPathFront + stageName + kStageDataPathBack).c_str());

	//読み込むオブジェクト数が何個あるか取得
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	//読み込むオブジェクト数分の配列に変更する
	m_trapPoss.resize(dataCnt);

	for (auto& trap : m_trapPoss)
	{
		trap = std::make_shared<Trap>();
		//座標を取得する
		FileRead_read(&trap->pos, sizeof(Vec3), handle);
		FileRead_read(&trap->norm, sizeof(Vec3), handle);
		trap->isPlaced = false;
	}

	FileRead_close(handle);
}

void TrapManager::SetUp(int point)
{
	for (auto& trap : m_trapPoss)
	{
		for (auto& temp : m_trapPoss)
		{
			if (abs((trap->pos - temp->pos).Length()) > 0.0f && abs((trap->pos - temp->pos).Length()) < 12.0f)
			{
				if (Dot(trap->norm.Normalize(),temp->norm.Normalize()) >= 1.0f)
				{
					trap->neighborTraps.emplace_back(temp);
				}
			}
		}
	}

	m_bgHandle = ResourceManager::GetInstance().GetHandle("I_TRAPPOINTBG");
	m_trapPoint = point;
}

void TrapManager::Clear()
{
	for (auto& trap : m_trapPoss)
	{
		trap->neighborTraps.clear();
	}
	m_trapPoss.clear();
	m_traps.clear();

	m_slotIdx = 0;
	m_cameraPos = Vec3();
	m_cameraDir = Vec3();
	m_trapPoint = 0;

	DeleteGraph(m_bgHandle);
	m_trapPoint = 0;
}

void TrapManager::EstablishTrap(Vec3 playerPos, Vec3 targetPos, int slot)
{
	if (!debugTrap->isPlaced && debugTrap->neighborTraps.size() == 8 && CheckNeighbor(debugTrap->neighborTraps))
	{
		//現状はスロット番号が1だったらスパイクトラップを設置する。
		//MEMO:今後、選択したトラップに応じた種類を設置するようにするかも
		if (slot == 1)
		{
			auto add = std::make_shared<SpikeTrap>();
			//もし設置しようとしていたトラップのコストよりも現在持っているポイントが少なかったら設置できない
			if (m_trapPoint < add->GetCost())
			{
				//何もしない
				return;
			}

			//所持トラップポイントをコスト分減らす
			m_trapPoint -= add->GetCost();

			//初期化
			add->Init(debugTrap->pos,debugTrap->norm);

			//追加
			m_traps.emplace_back(add);

			//トラップを設置済みにする
			debugTrap->isPlaced = true;
			for (auto& trap : debugTrap->neighborTraps)
			{
				trap.lock()->isPlaced = true;
			}
		}
	}
}

const void TrapManager::SetCameraInfo(Vec3 cameraPos, Vec3 dirVec)
{
	m_cameraPos = cameraPos;
	m_cameraDir = dirVec;
}

void TrapManager::SelectPoint(Vec3 playerPos, Vec3 targetPos)
{
#ifdef TRUE	//バウンディングボックスを使った処理方法
	//線分の始点と終点を設定
	//auto start = playerPos;
	auto start = m_cameraPos;
	auto end = playerPos + targetPos * 60;

	float defaultLength = 100.0f;

#ifdef _DEBUG
	DrawCube3D(start.ToVECTOR(), end.ToVECTOR(), 0xff0000, 0xff0000, false);
#endif
	//startとendを始点終点とした四角形との当たり判定をまずとる

	auto hit = CheckHitBoundingBoxAndPoints(start, end, m_trapPoss);

	//設置可能なトラップの座標分回す
	for (auto& trap : hit)
	{
		//トラップが置かれていない
		//if (!trap->isPlaced && trap->neighborTraps.size() == 8 && CheckNeighbor(trap->neighborTraps))
		{
			//線分とトラップ設置可能座標の距離を計算する
			//float length = Segment_Point_MinLength(start.ConvertToVECTOR(), end.ConvertToVECTOR(), pos.ConvertToVECTOR());
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap->pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;

				debugTrap = trap;
			}
		}
	}
#else		//バウンディングボックスを使わずにごり押し処理方法
	//線分の始点と終点を設定
	auto start = playerPos;
	auto end = playerPos + targetPos * 60;

	float defaultLength = 100.0f;

	//設置可能なトラップの座標分回す
	for (auto& trap : m_traps)
	{
		//トラップが置かれていない
		if (!trap->isPlaced && trap->neighborTraps.size() == 8 && CheckNeighbor(trap->neighborTraps))
		{
			//線分とトラップ設置可能座標の距離を計算する
			//float length = Segment_Point_MinLength(start.ConvertToVECTOR(), end.ConvertToVECTOR(), pos.ConvertToVECTOR());
			float length = Segment_Point_MinLength(start.ToVECTOR(), end.ToVECTOR(), trap->pos.ToVECTOR());

			if (defaultLength > length)
			{
				defaultLength = length;

				debugTrap = trap;
				//debugTrap->isPlaced = trap.isPlaced;
				//debugTrap->neighborTraps = trap.neighborTraps;
				//debugTrap->pos = trap.pos;
			}
		}
	}
#endif
}
