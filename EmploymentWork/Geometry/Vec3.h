#pragma once
#include <DxLib.h>
#include <cmath>

struct Vec3
{
public:
	float x, y, z;	//座標

public:
	/// <summary>
	/// 引数なしコンストラクタ
	/// </summary>
	Vec3();
	/// <summary>
	/// 引数ありコンストラクタ
	/// </summary>
	/// <param name="inX">X座標</param>
	/// <param name="inY">Y座標</param>
	/// <param name="inZ">Z座標</param>
	Vec3(float inX, float inY, float inZ);
	/// <summary>
	/// 引数ありコンストラクタ
	/// </summary>
	/// <param name="vec">DxLibのVECTOR型</param>
	Vec3(DxLib::VECTOR vec);

	/// <summary>
	/// 右ベクトル
	/// </summary>
	/// <returns>(1, 0, 0)</returns>
	static Vec3 Right();
	/// <summary>
	/// 左ベクトル
	/// </summary>
	/// <returns>(-1, 0, 0)</returns>
	static Vec3 Left();
	/// <summary>
	/// 上ベクトル
	/// </summary>
	/// <returns>(0, 1, 0)</returns>
	static Vec3 Up();
	/// <summary>
	/// 下ベクトル
	/// </summary>
	/// <returns>(0, -1, 0)</returns>
	static Vec3 Down();
	/// <summary>
	/// 正面ベクトル
	/// </summary>
	/// <returns>(0, 0, 1)</returns>
	static Vec3 Front();
	/// <summary>
	/// 背面ベクトル
	/// </summary>
	/// <returns>(0, 0, -1)</returns>
	static Vec3 Back();

	/* 演算 */
	/// <summary>
	/// 各値に-をかける
	/// </summary>
	Vec3 operator-() const;					
	/// <summary>
	/// 足し算
	/// </summary>
	Vec3 operator+(const Vec3& val) const;
	/// <summary>
	/// 足し算
	/// </summary>
	void operator+=(const Vec3& val);
	/// <summary>
	/// 引き算
	/// </summary>
	Vec3 operator-(const Vec3& val) const;
	/// <summary>
	/// 引き算
	/// </summary>
	void operator-=(const Vec3& val);
	/// <summary>
	/// 掛け算
	/// </summary>
	Vec3 operator*(float scale) const;
	/// <summary>
	/// 掛け算
	/// </summary>
	void operator*=(float scale);
	/// <summary>
	/// 割り算
	/// </summary>
	Vec3 operator/(float scale) const;
	/// <summary>
	/// 割り算
	/// </summary>
	void operator/=(float scale);
	/// <summary>
	/// 比較
	/// </summary>
	bool operator==(const Vec3& val)const;
	/// <summary>
	/// 比較
	/// </summary>
	bool operator!=(const Vec3& val)const;

	/// <summary>
	/// 長さを取得
	/// </summary>
	/// <returns>長さ</returns>
	float Length()const;

	/// <summary>
	/// 長さの二乗を取得
	/// </summary>
	/// <returns>長さの二乗</returns>
	float SqLength()const;

	/// <summary>
	/// 正規化ベクトルを取得
	/// </summary>
	/// <returns>正規化ベクトル</returns>
	Vec3 Normalize()const;

	/// <summary>
	/// DxLibのVECTOR型に変換
	/// </summary>
	/// <returns></returns>
	VECTOR ToVECTOR()const;
};

/// <summary>
/// 内積
/// </summary>
/// <param name="item1">ベクトル１</param>
/// <param name="item2">ベクトル２</param>
/// <returns>内積結果</returns>
float Dot(const Vec3& item1, const Vec3& item2);
/// <summary>
/// 外積
/// </summary>
/// <param name="item1">ベクトル１</param>
/// <param name="item2">ベクトル２</param>
/// <returns>外積結果</returns>
Vec3 Cross(const Vec3& item1, const Vec3& item2);

//namespace MyLib
//{
//	/// <summary>
//	/// 三次元ベクトルを扱うクラス
//	/// </summary>
//	class Vec3
//	{
//	public:
//		float x, y, z;	//座標
//
//	public:
//		/// <summary>
//		/// 引数なしコンストラクタ
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
//		/// 引数有りコンストラクタ
//		/// </summary>
//		/// <param name="x">X座標</param>
//		/// <param name="y">Y座標</param>
//		/// <param name="z">Z座標</param>
//		Vec3(const float x, const float y, const float z)
//		{
//			this->x = x;
//			this->y = y;
//			this->z = z;
//		}
//
//		/// <summary>
//		/// 足し算
//		/// </summary>
//		/// <param name="other">足すベクトル</param>
//		/// <returns>自身に引数のベクトルを足した結果ベクトル</returns>
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
//		/// 足し算
//		/// </summary>
//		/// <param name="other">足すベクトル</param>
//		/// <returns>自身に引数のベクトルを足した結果ベクトル</returns>
//		Vec3 operator+=(const Vec3& other)
//		{
//			this->x += other.x;
//			this->y += other.y;
//			this->z += other.z;
//			return *this;
//		}
//
//		/// <summary>
//		/// 引き算
//		/// </summary>
//		/// <param name="other">引くベクトル</param>
//		/// <returns>自身から引数のベクトルを引いた結果ベクトル</returns>
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
//		/// 引き算
//		/// </summary>
//		/// <param name="other">引くベクトル</param>
//		/// <returns>自身から引数のベクトルを引いた結果ベクトル</returns>
//		Vec3 operator-=(const Vec3& other)
//		{
//			this->x -= other.x;
//			this->y -= other.y;
//			this->z -= other.z;
//			return *this;
//		}
//
//		/// <summary>
//		/// 掛け算
//		/// </summary>
//		/// <param name="scale">かける数</param>
//		/// <returns>自身のそれぞれにかける数を掛けた結果ベクトル</returns>
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
//		/// 代入
//		/// </summary>
//		/// <param name="vec">代入するベクトル</param>
//		/// <returns>代入結果ベクトル</returns>
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
//		/// 比較
//		/// </summary>
//		/// <param name="other">比較対象ベクトル</param>
//		/// <returns>比較結果　false : 同一でない, true : 同一</returns>
//		bool operator==(const Vec3& other)const
//		{
//			bool ret;
//			ret = this->x == other.x && this->y == other.y && this->z == other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// 比較
//		/// </summary>
//		/// <param name="other">比較対象ベクトル</param>
//		/// <returns>比較結果　false : 同一, true : 同一でない</returns>
//		bool operator!=(const Vec3& other)const
//		{
//			bool ret;
//			ret = this->x != other.x || this->y != other.y || this->z != other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// ベクトルのサイズを取得する
//		/// </summary>
//		/// <returns>ベクトルのサイズ</returns>
//		float Length()const
//		{
//			float ret;
//			ret = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
//			return ret;
//		}
//
//		/// <summary>
//		/// ベクトルのサイズの2乗を取得する
//		/// </summary>
//		/// <returns>ベクトルのサイズの2乗</returns>
//		float SqLength()const
//		{
//			float ret;
//			ret = this->x * this->x + this->y * this->y + this->z * this->z;
//			return ret;
//		}
//
//		/// <summary>
//		/// 正規化ベクトルを取得する
//		/// </summary>
//		/// <returns>正規化ベクトル</returns>
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
//		/// 二つのベクトルの内積を取得する
//		/// </summary>
//		/// <param name="other">乗ベクトル</param>
//		/// <returns>内積</returns>
//		float Dot(const Vec3& other)const
//		{
//			float ret;
//			ret = this->x * other.x + this->y * other.y + this->z * other.z;
//			return ret;
//		}
//
//		/// <summary>
//		/// 二つのベクトルの外積を取得する
//		/// </summary>
//		/// <param name="other">乗ベクトル</param>
//		/// <returns>外積</returns>
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
//		/// DxLibのVECTOR型に変換
//		/// </summary>
//		/// <returns>DxLib::VECTOR型ベクトル</returns>
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