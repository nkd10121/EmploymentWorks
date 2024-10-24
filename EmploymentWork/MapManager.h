#pragma once
#include "Vec3.h"
#include <string>
#include <vector>
#include <unordered_map>

class MapManager
{
private:
	// �V���O���g���p�^�[���Ȃ̂ŃR���X�g���N�^��private�ɒu��
	MapManager();
	//�R���X�g���N�^
	virtual ~MapManager();

	//static�ɂ��邱�Ƃ�
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static MapManager* m_instance;

	//�z�u���f�[�^
	struct LocationData
	{
		std::string name;	//���O
		std::string tag;	//�^�O
		MyLib::Vec3 pos;	//���W
		MyLib::Vec3 rot;	//��]
		MyLib::Vec3 scale;	//�傫��
		int handle;
	};

	struct EnemyRoute
	{
		std::vector<MyLib::Vec3> start;
		std::vector<std::vector<MyLib::Vec3>> pos;
		MyLib::Vec3 end;
	};

public:
	//�R�s�[�R���X�g���N�^������̂̐������ł��Ă��܂�����
	//�R�s�[�R���X�g���N�^���֎~����
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
	MapManager(MapManager&&) = delete;
	MapManager& operator= (const MapManager&&) = delete;

	/// <summary>
	/// MapManager��GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static MapManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new MapManager;
		}

		return *m_instance;
	}

	//��������Y���ƕ��ʂɃ��������[�N
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	void Init();

	//�X�e�[�W�̐����ɕK�v�ȃ��f���̓ǂݍ���
	void LoadModel();
	//�X�e�[�W�̐����ɕK�v�ȃ��f���̍폜
	void DeleteModel();

	//�X�e�[�W����ǂݍ���
	void Load(const char* stageName);
	//�X�e�[�W��`�悷��
	void Draw();

private:
	//�}�b�v���
	std::vector<LocationData> m_data;
	//���f���n���h��
	std::unordered_map<std::string, int> m_handles;
};

