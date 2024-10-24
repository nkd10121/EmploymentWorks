#include "ModelManager.h"
#include "DxLib.h"
#include <cassert>

ModelManager* ModelManager::m_instance = nullptr;

/// <summary>
/// �f�X�g���N�^
/// </summary>
ModelManager::~ModelManager()
{
	//�����Ă���n���h����S��Delete����
	for (auto& h : m_handles)
	{
		MV1DeleteModel(h.handle);
	}

	m_handles.clear();
}

/// <summary>
/// �w�肵���p�X�����f�������[�h����
/// </summary>
void ModelManager::LoadModel(std::string path, bool isEternal)
{
	//���łɃ��[�h����Ă����牽�����Ȃ�
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
#ifdef _DEBUG
			//�w�肵���p�X�̃��f�������łɃ��[�h����Ă�����G���[��f���悤�ɂ���
			assert(0 && "���̃��f���͂��łɃ��[�h����Ă��܂�");
#endif
			return;
		}
	}

	//�����ɗ����Ƃ������Ƃ͂��łɃ��[�h����Ă��Ȃ�����
	//���V�������[�h����K�v������
	Model add;
	add.handle = MV1LoadModel(path.c_str());
	add.path = path;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

#ifdef _DEBUG
	//���f���̃��[�h�Ɏ��s���Ă�����G���[��f���悤�ɂ���
	assert(add.handle != -1 && "���f���̃��[�h�Ɏ��s���܂���");
#endif
	return;
}

/// <summary>
/// ���f���n���h�����擾����
/// </summary>
int ModelManager::GetModelHandle(std::string path)
{
	//���[�h����Ă����畡���n���h����Ԃ�
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
			return MV1DuplicateModel(h.handle);
		}
	}

	//�����܂ŗ����Ƃ������Ƃ̓��[�h����Ă��Ȃ�����
#ifdef _DEBUG
	//�O�̂���assert���d����ł���
	assert(0 && "�w�肵���p�X�̓��[�h����Ă��܂���");
#endif
	return -1;
}

/// <summary>
/// �풓�t���O��false�̃n���h�����폜����
/// </summary>
void ModelManager::Clear()
{
	//isEternal��false�̃n���h����Delete����
	for (auto& h : m_handles)
	{
		if (!h.isEternal)
		{
			MV1DeleteModel(h.handle);
		}
	}

	//�s�v�ɂȂ����n���h���������ō폜��������
	auto it = remove_if(m_handles.begin(), m_handles.end(), [](auto& v) {
		return v.isEternal == false;
		});
	m_handles.erase(it, m_handles.end());
}

/// <summary>
/// �n���h�����ǂݍ��܂�Ă��邩�ǂ����m�F
/// </summary>
bool ModelManager::IsLoaded()
{
	for (auto& h : m_handles)
	{
		if (CheckHandleASyncLoad(h.handle))	return false;
	}

	return true;
}