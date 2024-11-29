#include "EnemyManager.h"

#include "EnemyNormal.h"
#include "SwarmEnemy.h"

#include "LoadCSV.h"

namespace
{
	const unsigned int kColor[6]
	{
		0xffffff,
		0x000000,
		0xff0000,
		0x00ff00,
		0x0000ff,
		0xffff00
	};
}

EnemyManager::EnemyManager():
	m_deadEnemyNum(0)
{
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Init(std::string stageName)
{
	auto info = LoadCSV::GetInstance().LoadEnemyCreateData(stageName);
	for (auto& inf : info)
	{
		SeparateData(inf);
	}

	for (auto& inf : m_createEnemyInfo)
	{
		m_enemyNum[inf.first] = inf.second.size();
	}
}

bool EnemyManager::Update(int phase)
{
	//敵の更新
	for (auto& enemy : m_pEnemies)
	{
		enemy->Update();
	}
	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_pEnemies.begin(), m_pEnemies.end(), [](auto& v)
			{
				return v->GetIsExistMember() == false;
			});
		m_pEnemies.erase(it, m_pEnemies.end());
	}

	//もし群れの数が0になった(敵が全滅した)ら、次のフェーズに行く
	if (m_pEnemies.size() == 0)
	{
		return true;
	}

	return false;
}

void EnemyManager::Draw()
{
	for (auto& enemy : m_pEnemies)
	{
		enemy->Draw();
	}
}

void EnemyManager::Finalize()
{
	for (auto& enemy : m_pEnemies)
	{
		enemy->Finalize();
	}
	m_pEnemies.clear();
}

void EnemyManager::UpdateModelPos()
{
	for (auto& enemy : m_pEnemies)
	{
		enemy->UpdateModelPos();
	}
}

void EnemyManager::CreateEnemy(int phaseNum)
{
	int i = 0;
	auto addSwarm = std::make_shared<SwarmEnemy>(kColor[i]);

	for (auto& data : m_createEnemyInfo[phaseNum])
	{
		if (data.enemyName == "EnemyNormal")
		{
			auto add = std::make_shared<EnemyNormal>();
			add->SetPos(Vec3(-48.0f + 16 * i, 8.0f, -48.0f));
			add->Init();
			addSwarm->AddSwarm(add);

			data.isCreated = true;
		}

		i++;
	}

	addSwarm->SetUp();
	m_pEnemies.emplace_back(addSwarm);

	////DEBUG:敵を生成
	//for (int i = 0; i < 1; i++)
	//{
	//	auto addSwarm = std::make_shared<SwarmEnemy>(kColor[i]);

	//	for (int j = 0; j < 1; j++)
	//	{
	//		auto add = std::make_shared<EnemyNormal>();
	//		add->SetPos(Vec3(-48.0f + 16 * i, 8.0f, -48.0f + 16 * j));
	//		add->Init();

	//		addSwarm->AddSwarm(add);

	//	}

	//	addSwarm->SetUp();
	//	m_pEnemies.emplace_back(addSwarm);
	//}
}

void EnemyManager::SeparateData(std::vector<std::string> data)
{
	EnemyCreateInfo add;
	add.enemyName = data[1];
	add.isCreated = false;

	m_createEnemyInfo[std::stoi(data[0])].push_back(add);
}
