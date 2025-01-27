#include "ScoreManager.h"

ScoreManager* ScoreManager::m_instance = nullptr;

ScoreManager::ScoreManager():
	m_clearTime(0),
	m_playerKillNum(0),
	m_trapKillNum(0),
	m_crystalHp(0),
	m_maxComboNum(0),
	m_score(0)
{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Clear()
{
	m_clearTime = 0;
	m_playerKillNum = 0;
	m_trapKillNum = 0;
	m_crystalHp = 0;
	m_maxComboNum = 0;
	//m_score = 0;
}

const void ScoreManager::CalculationScore(std::string stageName)
{
	//タイムの計算
	int score = 0;
	score += (m_targetClearTime - m_clearTime) / 6;
	score += m_playerKillNum * 50;
	score += m_trapKillNum * 100;
	score += m_crystalHp + 100;
	//m_score += m_maxCombo * 100;

	auto it = m_score.find(stageName);
	//もし、ステージ名のスコアが保存されていたら
	if (it != m_score.end())
	{
		//もともとあったスコアと比較して大きかったら上書きする
		m_score[stageName] = std::max(m_score[stageName], score);
	}
	else
	{
		m_score[stageName] = score;
	}
}
