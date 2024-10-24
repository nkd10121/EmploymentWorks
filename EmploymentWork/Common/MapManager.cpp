#include "MapManager.h"
#include <fstream>
#include <sstream>

#include "ModelManager.h"

MapManager* MapManager::m_instance = nullptr;

namespace
{
	float DegreeToRadian(float deg)
	{
		return deg * DX_PI_F / 180.0f;
	}

	const std::string kModelPathFront = "data/model/stage/";
	const std::string kModelPathBack = ".mv1";

	const std::string kStageDataPathFront = "data/stageData/";
	const std::string kStageDataPathBack = ".loc";
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapManager::MapManager()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// ������
/// </summary>
void MapManager::Init()
{
	//���[�h���������Ă���Ƃ��ɂ������Ă΂�邽�߁A���f���}�l�[�W���[�ɗ���ł��������f�����擾����
	m_handles["floor"] = ModelManager::GetInstance().GetModelHandle((kModelPathFront + "floor" + kModelPathBack));
	m_handles["wall"] = ModelManager::GetInstance().GetModelHandle((kModelPathFront + "wall" + kModelPathBack));
	m_handles["doorFrame"] = ModelManager::GetInstance().GetModelHandle((kModelPathFront + "doorFrame" + kModelPathBack));

	m_handles["crystal"] = ModelManager::GetInstance().GetModelHandle((kModelPathFront + "crystal" + kModelPathBack));
	m_handles["Blocks"] = ModelManager::GetInstance().GetModelHandle((kModelPathFront + "Blocks" + kModelPathBack));
}

/// <summary>
/// �X�e�[�W�̐����ɕK�v�ȃ��f���̓ǂݍ���
/// </summary>
void MapManager::LoadModel()
{
	ModelManager::GetInstance().LoadModel((kModelPathFront + "floor" + kModelPathBack));
	ModelManager::GetInstance().LoadModel((kModelPathFront + "wall" + kModelPathBack));
	ModelManager::GetInstance().LoadModel((kModelPathFront + "doorFrame" + kModelPathBack));

	ModelManager::GetInstance().LoadModel((kModelPathFront + "crystal" + kModelPathBack));
	ModelManager::GetInstance().LoadModel((kModelPathFront + "Blocks" + kModelPathBack));
}

/// <summary>
/// �X�e�[�W�̐����ɕK�v�ȃ��f���̍폜
/// </summary>
void MapManager::DeleteModel()
{
	//���f���̍폜
	for (auto& model : m_handles)
	{
		MV1DeleteModel(model.second);
	}
	m_handles.clear();

	//���f���ƃf�[�^�̍폜
	for (auto& data : m_data)
	{
		MV1DeleteModel(data.handle);
	}
	m_data.clear();
}

/// <summary>
/// �X�e�[�W����ǂݍ���
/// </summary>
void MapManager::Load(const char* stageName)
{
	//���f���̃��[�h
	//MapManager::LoadModel();

	//�J���t�@�C���̃n���h�����擾
	int handle = FileRead_open((kStageDataPathFront + stageName + kStageDataPathBack).c_str());

	//�ǂݍ��ރI�u�W�F�N�g���������邩�擾
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	//�ǂݍ��ރI�u�W�F�N�g�����̔z��ɕύX����
	m_data.resize(dataCnt);

	//�z��̐�����
	for (auto& loc : m_data)
	{
		//���O�̃o�C�g�����擾����
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		//���O�̃T�C�Y��ύX����
		loc.name.resize(nameCnt);
		//���O���擾����
		FileRead_read(loc.name.data(), sizeof(char) * static_cast<int>(loc.name.size()), handle);

		//�^�O�̃o�C�g�����擾����
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), handle);
		//�^�O�̃T�C�Y��ύX����
		loc.tag.resize(tagCnt);
		//�^�O���擾����
		FileRead_read(loc.tag.data(), sizeof(char) * static_cast<int>(loc.tag.size()), handle);

		//���W���擾����
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);
		//��]���擾����
		FileRead_read(&loc.rot, sizeof(loc.rot), handle);
		//�傫�����擾����
		FileRead_read(&loc.scale, sizeof(loc.scale), handle);

		//if (loc.tag == "trapPos")
		//{
		//	m_trapPos.push_back(loc.pos);
		//}

		//if (loc.tag == "crystal")
		//{
		//	m_crustalPos = loc.pos;
		//}
		//LoadEnemyRoute(loc);

	}
	FileRead_close(handle);

	for (auto& loc : m_data)
	{
		loc.rot.x = DegreeToRadian(loc.rot.x);
		loc.rot.y = DegreeToRadian(loc.rot.y);
		loc.rot.z = DegreeToRadian(loc.rot.z);

		loc.handle = MV1DuplicateModel(m_handles[loc.tag]);
		MV1SetPosition(loc.handle, loc.pos.ConvertToVECTOR());
		MV1SetScale(loc.handle, loc.scale.ConvertToVECTOR());
		MV1SetRotationXYZ(loc.handle, loc.rot.ConvertToVECTOR());

	}
}

/// <summary>
/// �`��
/// </summary>
void MapManager::Draw()
{
	for (auto& loc : m_data)
	{
		if (loc.tag != "floor" && loc.tag != "wall" && loc.tag != "doorFrame" && loc.tag != "crystal" && loc.tag != "Blocks")
		{
			continue;
		}

		MV1DrawModel(loc.handle);
	}
}
