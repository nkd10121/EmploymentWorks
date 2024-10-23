#include "LoadCSV.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

LoadCSV* LoadCSV::m_instance = nullptr;

namespace
{
	// csvデータの,で文字列を分割する
	std::vector<std::string> Split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter)) {
			result.push_back(field);
		}
		return result;
	}

	const std::string kFrontPathName = "data/csv/";
	const std::string kBackPathName = ".csv";

	namespace LoadData
	{
		//ステータス情報の並び列挙型
		enum eStatusOrder : int
		{
			name,	//キャラクター名
			hp,		//体力
			atk,	//攻撃力
			def,	//防御力
			speed,	//移動速度
			point	//ドロップする罠ポイント
		};
	}
}

/// <summary>
/// ステータス情報を取得
/// </summary>
CharacterBase::Status LoadCSV::StatusLoad(const char* characterName)
{
	//返す情報
	CharacterBase::Status retStatus;

	// 一時保存用string
	std::string strBuf;
	// カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	// ファイル読み込み
	std::ifstream ifs("data/csv/status.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	//情報を取得できたかどうかのフラグ
	bool isGet = false;

	//最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//取得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//[0]:キャラクター名
		//[1]:体力
		//[2]:攻撃力
		//[3]:防御力
		//[4]:移動速度
		//[5]:死亡時のポイント

		//指定したキャラクター名と一致するデータがあれば情報を取得する
		if (strConmaBuf[LoadData::eStatusOrder::name] == characterName)
		{
			isGet = true;
			retStatus.hp = std::stoi(strConmaBuf[LoadData::eStatusOrder::hp]);
			retStatus.atk = std::stoi(strConmaBuf[LoadData::eStatusOrder::atk]);
			retStatus.def = std::stoi(strConmaBuf[LoadData::eStatusOrder::def]);
			retStatus.speed = std::stof(strConmaBuf[LoadData::eStatusOrder::speed]);
			retStatus.point = std::stoi(strConmaBuf[LoadData::eStatusOrder::point]);
		}
	}

#ifdef _DEBUG
	//情報を取得できなかった時、エラーを吐くようにする
	//取得できなかった理由想定
	//①キャラクター名がスペルミスや大文字小文字の違いで異なる
	//②csvに情報を追加し忘れている
	if (!isGet)
	{
		assert(0 && "指定したキャラクター名の情報を取得できませんでした");
	}
#endif
	return retStatus;
}
