#pragma once
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>

class SwarmEnemy;	//敵

class EnemyManager
{
public:
	/// <summary>
	/// 敵生成情報
	/// </summary>
	struct EnemyCreateInfo
	{
		std::string enemyName;		//敵の名前
		//int appearFrame;			//出現フレーム
		//int appearPos;			//生成場所(現在は未使用)

		bool isCreated = false;		//生成済みかどうか
	};
public:
	EnemyManager();
	virtual ~EnemyManager();

	void Init(std::string stageName);
	bool Update(int phase);
	void Draw();

	void Finalize();

	void UpdateModelPos();

	void CreateEnemy(int phaseNum);

	void SeparateData(std::vector<std::string> data);

private:
	std::list<std::shared_ptr<SwarmEnemy>> m_pEnemies;	//敵ポインタの配列
	std::map<int, std::list<EnemyCreateInfo>> m_createEnemyInfo;
	std::unordered_map<int, int> m_enemyNum;	//それぞれのフェーズで何体の敵を生成する予定か保存

	int m_deadEnemyNum;
};