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
	m_score = 0;
}

const void ScoreManager::CalculationScore()
{
	return void();
}
