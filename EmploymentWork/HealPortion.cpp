#include "HealPortion.h"
#include "DxLib.h"
#include "ModelManager.h"

namespace
{
	//�����蔻��̉~�̔��a
	constexpr float kCollisionRadius = 2.4f;

	//���f���T�C�Y
	constexpr float kModelScale = 0.08f;
	//�G�t�F�N�g�𐶐�����Ԋu
	constexpr int kCreateEffectInterval = 120;
	//�|�[�V�������㉺���鑬�x
	constexpr float kMoveSpeed = 0.04f;
	//�|�[�V�������㉺���镝
	constexpr float kMoveRange = 0.01f;
	//���W��ݒ肷��Ƃ���Y���W�̃I�t�Z�b�g
	constexpr float kOffsetPosY = 2.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
HealPortion::HealPortion() :
	ObjectBase(Collidable::Priority::Low, GameObjectTag::Portion),
	m_isExist(false),
	m_effectCreateFrame(0),
	m_angle(0.0f),
	m_posOffsetY(0.0f)
{
	//�����蔻��̐���
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kCollisionRadius;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HealPortion::~HealPortion()
{
	//���\�[�X�J��
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
void HealPortion::Init(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Init(physics);

	//���������̏�����
	rigidbody->Init();

	//���f���̃n���h�����擾
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("data/model/object/portion/bottle_red.mv1");
	//���f���̃X�P�[����ݒ�
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));

	//���݃t���O��true�ɂ���
	m_isExist = true;
}

/// <summary>
/// �����蔻����폜
/// </summary>
void HealPortion::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Finalize(physics);
}

/// <summary>
/// �X�V
/// </summary>
void HealPortion::Update()
{
	//���݂��Ă��Ȃ���ԂȂ牽�������Ȃ�
	if (!m_isExist)return;

	//��莞�Ԃ��ƂɃG�t�F�N�g���o��
	if (m_effectCreateFrame % kCreateEffectInterval == 0)
	{
		//�G�t�F�N�g�̕\�����W���擾
		auto pos = rigidbody->GetPos();
		//�G�t�F�N�g�𐶐�
		//EffectManager::GetInstance().CreateEffect("Portion", pos, pos);
	}

	//�G�t�F�N�g�����J�E���g�𑝂₷
	m_effectCreateFrame++;

	/*�|�[�V������Y�����sin�J�[�u�ŏ㉺���Ȃ���AY����]���邽�߂̏���*/
	m_angle += kMoveSpeed;
	m_posOffsetY = sinf(m_angle) * kMoveRange;

	auto pos = rigidbody->GetPos();
	pos.y += m_posOffsetY;

	rigidbody->SetPos(pos);
	MV1SetPosition(m_modelHandle, pos.ConvertToVECTOR());
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle / 2, 0.0f));
}

/// <summary>
/// �`��
/// </summary>
void HealPortion::Draw()
{
	//���݂��Ă��Ȃ���ԂȂ牽�������Ȃ�
	if (!m_isExist)return;

	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}

/// <summary>
/// ���W��ݒ�
/// </summary>
void HealPortion::SetPosition(MyLib::Vec3 pos)
{
	pos.y += kOffsetPosY;
	rigidbody->SetPos(pos);

	MV1SetPosition(m_modelHandle, rigidbody->GetPosVECTOR());
}

/// <summary>
/// �I��������
/// </summary>
void HealPortion::End()
{
	m_isExist = false;
}