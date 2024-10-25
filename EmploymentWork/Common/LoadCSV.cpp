#include "LoadCSV.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

LoadCSV* LoadCSV::m_instance = nullptr;

namespace
{
	// csv�f�[�^��,�ŕ�����𕪊�����
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
		//�X�e�[�^�X���̕��ї񋓌^
		enum eStatusOrder : int
		{
			name,	//�L�����N�^�[��
			hp,		//�̗�
			atk,	//�U����
			def,	//�h���
			speed,	//�ړ����x
			point	//�h���b�v����㩃|�C���g
		};
	}
}

/// <summary>
/// �X�e�[�^�X�����擾
/// </summary>
CharacterBase::Status LoadCSV::LoadStatus(const char* characterName)
{
	//�Ԃ����
	CharacterBase::Status retStatus;

	// �ꎞ�ۑ��pstring
	std::string strBuf;
	// �J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	// �t�@�C���ǂݍ���
	std::ifstream ifs("data/csv/status.csv");
	if (!ifs)
	{
		assert(0 && "�t�@�C���ɃA�N�Z�X�ł��܂���ł����B");
		return CharacterBase::Status();
	}

	//�����擾�ł������ǂ����̃t���O
	bool isGet = false;

	//�ŏ��͑Ή��\��񂪓����Ă��邾���Ȃ̂Ŗ�������
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//�擾������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//[0]:�L�����N�^�[��
		//[1]:�̗�
		//[2]:�U����
		//[3]:�h���
		//[4]:�ړ����x
		//[5]:���S���̃|�C���g

		//�w�肵���L�����N�^�[���ƈ�v����f�[�^������Ώ����擾����
		if (strConmaBuf[LoadData::eStatusOrder::name] == characterName)
		{
			isGet = true;
			retStatus.hp = std::stoi(strConmaBuf[LoadData::eStatusOrder::hp]);
			retStatus.atk = std::stoi(strConmaBuf[LoadData::eStatusOrder::atk]);
			retStatus.def = std::stoi(strConmaBuf[LoadData::eStatusOrder::def]);
			retStatus.speed = std::stof(strConmaBuf[LoadData::eStatusOrder::speed]);
			retStatus.point = std::stoi(strConmaBuf[LoadData::eStatusOrder::point]);
		}

		if (isGet)
		{
			break;
		}
	}

#ifdef _DEBUG
	//�����擾�ł��Ȃ��������A�G���[��f���悤�ɂ���
	//�擾�ł��Ȃ��������R�z��
	//�@�L�����N�^�[�����X�y���~�X��啶���������̈Ⴂ�ňقȂ�
	//�Acsv�ɏ���ǉ����Y��Ă���
	if (!isGet)
	{
		assert(0 && "�w�肵���L�����N�^�[���̏����擾�ł��܂���ł���");
	}
#endif
	return retStatus;
}

std::map<std::string, std::string> LoadCSV::GetLoadResourcePath(std::string stageId)
{
	//�߂�l
	std::map<std::string, std::string> ret;

	// �ꎞ�ۑ��pstring
	std::string strBuf;
	// �J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	// �t�@�C���ǂݍ���
	std::ifstream ifs("data/csv/LoadPath.csv");
	if (!ifs)
	{
		assert(0 && "�t�@�C���ɃA�N�Z�X�ł��܂���ł����B");
		return ret;
	}

	//�ŏ��͑Ή��\��񂪓����Ă��邾���Ȃ̂Ŗ�������
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//�擾������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//[0] ID
		//[1] �p�X
		//[2] �g���q

		//�w�肵��ID�ƈ�v���Ă�����
		if (stageId == strConmaBuf[0])
		{
			//�߂�l�̕ϐ��Ƀf�[�^��ǉ�����
			ret[strConmaBuf[1]] = strConmaBuf[2];
		}
	}

	return ret;
}