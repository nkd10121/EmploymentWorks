#pragma once
#include "Vec3.h"
#include <string>
#include <vector>
#include <unordered_map>

/// <summary>
/// �}�b�v���Ǘ�����N���X
/// </summary>
class MapManager
{
private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	MapManager();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~MapManager();

	//static�ɂ��邱�Ƃ�
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static MapManager* m_instance;

	//�z�u���f�[�^
	struct LocationData
	{
		std::string name;	//���O
		std::string tag;	//�^�O
		Vec3 pos;	//���W
		Vec3 rot;	//��]
		Vec3 scale;	//�傫��
		int handle;
	};

	struct EnemyRoute
	{
		std::vector<Vec3> start;
		std::vector<std::vector<Vec3>> pos;
		Vec3 end;
	};

public:
	//�R�s�[�R���X�g���N�^������̂̐������ł��Ă��܂�����
	//�R�s�[�R���X�g���N�^���֎~����
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
	MapManager(MapManager&&) = delete;
	MapManager& operator= (const MapManager&&) = delete;

	/// <summary>
	/// �C���X�^���X���擾
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

	/// <summary>
	/// �폜
	/// ��������Y���ƕ��ʂɃ��������[�N
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�e�[�W�̐����ɕK�v�ȃ��f���̓ǂݍ���
	/// </summary>
	void LoadModel();
	/// <summary>
	/// �X�e�[�W�̐����ɕK�v�ȃ��f���̍폜
	/// </summary>
	void DeleteModel();

	/// <summary>
	/// �X�e�[�W����ǂݍ���
	/// </summary>
	/// <param name="stageName">�X�e�[�W��</param>
	void Load(const char* stageName);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	std::vector<LocationData> m_data;	//�}�b�v���
	std::unordered_map<std::string, int> m_handles;	//���f���n���h��
};

