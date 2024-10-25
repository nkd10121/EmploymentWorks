#pragma once
#include <DxLib.h>
#include <cmath>

struct Vec3
{
public:
	float x, y, z;	//���W

public:
	/// <summary>
	/// �����Ȃ��R���X�g���N�^
	/// </summary>
	Vec3();
	/// <summary>
	/// ��������R���X�g���N�^
	/// </summary>
	/// <param name="inX">X���W</param>
	/// <param name="inY">Y���W</param>
	/// <param name="inZ">Z���W</param>
	Vec3(float inX, float inY, float inZ);

	/// <summary>
	/// �E�x�N�g��
	/// </summary>
	/// <returns>(1, 0, 0)</returns>
	static Vec3 Right();
	/// <summary>
	/// ���x�N�g��
	/// </summary>
	/// <returns>(-1, 0, 0)</returns>
	static Vec3 Left();
	/// <summary>
	/// ��x�N�g��
	/// </summary>
	/// <returns>(0, 1, 0)</returns>
	static Vec3 Up();
	/// <summary>
	/// ���x�N�g��
	/// </summary>
	/// <returns>(0, -1, 0)</returns>
	static Vec3 Down();
	/// <summary>
	/// ���ʃx�N�g��
	/// </summary>
	/// <returns>(0, 0, 1)</returns>
	static Vec3 Front();
	/// <summary>
	/// �w�ʃx�N�g��
	/// </summary>
	/// <returns>(0, 0, -1)</returns>
	static Vec3 Back();

	/* ���Z */
	/// <summary>
	/// �e�l��-��������
	/// </summary>
	Vec3 operator-() const;					
	/// <summary>
	/// �����Z
	/// </summary>
	Vec3 operator+(const Vec3& val) const;
	/// <summary>
	/// �����Z
	/// </summary>
	void operator+=(const Vec3& val);
	/// <summary>
	/// �����Z
	/// </summary>
	Vec3 operator-(const Vec3& val) const;
	/// <summary>
	/// �����Z
	/// </summary>
	void operator-=(const Vec3& val);
	/// <summary>
	/// �|���Z
	/// </summary>
	Vec3 operator*(float scale) const;
	/// <summary>
	/// �|���Z
	/// </summary>
	void operator*=(float scale);
	/// <summary>
	/// ����Z
	/// </summary>
	Vec3 operator/(float scale) const;
	/// <summary>
	/// ����Z
	/// </summary>
	void operator/=(float scale);
	/// <summary>
	/// ��r
	/// </summary>
	bool operator==(const Vec3& val)const;
	/// <summary>
	/// ��r
	/// </summary>
	bool operator!=(const Vec3& val)const;

	/// <summary>
	/// �������擾
	/// </summary>
	/// <returns>����</returns>
	float Length()const;

	/// <summary>
	/// �����̓����擾
	/// </summary>
	/// <returns>�����̓��</returns>
	float SqLength()const;

	/// <summary>
	/// ���K���x�N�g�����擾
	/// </summary>
	/// <returns>���K���x�N�g��</returns>
	Vec3 Normalize()const;

	/// <summary>
	/// DxLib��VECTOR�^�ɕϊ�
	/// </summary>
	/// <returns></returns>
	VECTOR ToVECTOR()const;
};

/// <summary>
/// ����
/// </summary>
/// <param name="item1">�x�N�g���P</param>
/// <param name="item2">�x�N�g���Q</param>
/// <returns>���ό���</returns>
float Dot(const Vec3& item1, const Vec3& item2);
/// <summary>
/// �O��
/// </summary>
/// <param name="item1">�x�N�g���P</param>
/// <param name="item2">�x�N�g���Q</param>
/// <returns>�O�ό���</returns>
Vec3 Cross(const Vec3& item1, const Vec3& item2);

//namespace MyLib
//{
//	/// <summary>
//	/// �O�����x�N�g���������N���X
//	/// </summary>
//	class Vec3
//	{
//	public:
//		float x, y, z;	//���W
//
//	public:
//		/// <summary>
//		/// �����Ȃ��R���X�g���N�^
//		/// </summary>
//		Vec3() :
//			x(0.0f),
//			y(0.0f),
//			z(0.0f)
//		{
//
//		}
//
//		/// <summary>
//		/// �����L��R���X�g���N�^
//		/// </summary>
//		/// <param name="x">X���W</param>
//		/// <param name="y">Y���W</param>
//		/// <param name="z">Z���W</param>
//		Vec3(const float x, const float y, const float z)
//		{
//			this->x = x;
//			this->y = y;
//			this->z = z;
//		}
//
//		/// <summary>
//		/// �����Z
//		/// </summary>
//		/// <param name="other">�����x�N�g��</param>
//		/// <returns>���g�Ɉ����̃x�N�g���𑫂������ʃx�N�g��</returns>
//		Vec3 operator+(const Vec3& other)const
//		{
//			Vec3 ret;
//			ret.x = this->x + other.x;
//			ret.y = this->y + other.y;
//			ret.z = this->z + other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// �����Z
//		/// </summary>
//		/// <param name="other">�����x�N�g��</param>
//		/// <returns>���g�Ɉ����̃x�N�g���𑫂������ʃx�N�g��</returns>
//		Vec3 operator+=(const Vec3& other)
//		{
//			this->x += other.x;
//			this->y += other.y;
//			this->z += other.z;
//			return *this;
//		}
//
//		/// <summary>
//		/// �����Z
//		/// </summary>
//		/// <param name="other">�����x�N�g��</param>
//		/// <returns>���g��������̃x�N�g�������������ʃx�N�g��</returns>
//		Vec3 operator-(const Vec3& other)const
//		{
//			Vec3 ret;
//			ret.x = this->x - other.x;
//			ret.y = this->y - other.y;
//			ret.z = this->z - other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// �����Z
//		/// </summary>
//		/// <param name="other">�����x�N�g��</param>
//		/// <returns>���g��������̃x�N�g�������������ʃx�N�g��</returns>
//		Vec3 operator-=(const Vec3& other)
//		{
//			this->x -= other.x;
//			this->y -= other.y;
//			this->z -= other.z;
//			return *this;
//		}
//
//		/// <summary>
//		/// �|���Z
//		/// </summary>
//		/// <param name="scale">�����鐔</param>
//		/// <returns>���g�̂��ꂼ��ɂ����鐔���|�������ʃx�N�g��</returns>
//		Vec3 operator*(const float scale)
//		{
//			Vec3 ret;
//			ret.x = this->x * scale;
//			ret.y = this->y * scale;
//			ret.z = this->z * scale;
//			return ret;
//		}
//
//		/// <summary>
//		/// ���
//		/// </summary>
//		/// <param name="vec">�������x�N�g��</param>
//		/// <returns>������ʃx�N�g��</returns>
//		Vec3 operator=(const VECTOR& vec)
//		{
//			Vec3 ret;
//			ret.x = vec.x;
//			ret.y = vec.y;
//			ret.z = vec.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// ��r
//		/// </summary>
//		/// <param name="other">��r�Ώۃx�N�g��</param>
//		/// <returns>��r���ʁ@false : ����łȂ�, true : ����</returns>
//		bool operator==(const Vec3& other)const
//		{
//			bool ret;
//			ret = this->x == other.x && this->y == other.y && this->z == other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// ��r
//		/// </summary>
//		/// <param name="other">��r�Ώۃx�N�g��</param>
//		/// <returns>��r���ʁ@false : ����, true : ����łȂ�</returns>
//		bool operator!=(const Vec3& other)const
//		{
//			bool ret;
//			ret = this->x != other.x || this->y != other.y || this->z != other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// �x�N�g���̃T�C�Y���擾����
//		/// </summary>
//		/// <returns>�x�N�g���̃T�C�Y</returns>
//		float Length()const
//		{
//			float ret;
//			ret = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
//			return ret;
//		}
//
//		/// <summary>
//		/// �x�N�g���̃T�C�Y��2����擾����
//		/// </summary>
//		/// <returns>�x�N�g���̃T�C�Y��2��</returns>
//		float SqLength()const
//		{
//			float ret;
//			ret = this->x * this->x + this->y * this->y + this->z * this->z;
//			return ret;
//		}
//
//		/// <summary>
//		/// ���K���x�N�g�����擾����
//		/// </summary>
//		/// <returns>���K���x�N�g��</returns>
//		Vec3 Normalize()const
//		{
//			Vec3 ret;
//			float size = this->Length();
//			if (size == 0.0f)
//			{
//				return ret;
//			}
//			ret.x = this->x / size;
//			ret.y = this->y / size;
//			ret.z = this->z / size;
//			return ret;
//		}
//
//		/// <summary>
//		/// ��̃x�N�g���̓��ς��擾����
//		/// </summary>
//		/// <param name="other">��x�N�g��</param>
//		/// <returns>����</returns>
//		float Dot(const Vec3& other)const
//		{
//			float ret;
//			ret = this->x * other.x + this->y * other.y + this->z * other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// ��̃x�N�g���̊O�ς��擾����
//		/// </summary>
//		/// <param name="other">��x�N�g��</param>
//		/// <returns>�O��</returns>
//		Vec3 Cross(const Vec3& other)const
//		{
//			Vec3 ret;
//			ret.x = this->y * other.z - this->z * other.y;
//			ret.y = this->z * other.x - this->x * other.z;
//			ret.z = this->x * other.y - this->y * other.x;
//			return ret;
//		}
//
//		/// <summary>
//		/// DxLib��VECTOR�^�ɕϊ�
//		/// </summary>
//		/// <returns>DxLib::VECTOR�^�x�N�g��</returns>
//		DxLib::VECTOR ConvertToVECTOR()
//		{
//			DxLib::VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
//			ret.x = this->x;
//			ret.y = this->y;
//			ret.z = this->z;
//			return ret;
//		}
//	};
//}