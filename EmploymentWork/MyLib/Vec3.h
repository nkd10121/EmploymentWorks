#pragma once
#include <DxLib.h>
#include <cmath>

namespace MyLib
{
	/// <summary>
	/// 三次元ベクトルを扱うクラス
	/// </summary>
	class Vec3
	{
	public:
		float x, y, z;	//座標

	public:
		/// <summary>
		/// 引数なしコンストラクタ
		/// </summary>
		Vec3() :
			x(0.0f),
			y(0.0f),
			z(0.0f)
		{

		}

		/// <summary>
		/// 引数有りコンストラクタ
		/// </summary>
		/// <param name="x">X座標</param>
		/// <param name="y">Y座標</param>
		/// <param name="z">Z座標</param>
		Vec3(const float x, const float y, const float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/// <summary>
		/// 足し算
		/// </summary>
		/// <param name="other">足すベクトル</param>
		/// <returns>自身に引数のベクトルを足した結果ベクトル</returns>
		Vec3 operator+(const Vec3& other)const
		{
			Vec3 ret;
			ret.x = this->x + other.x;
			ret.y = this->y + other.y;
			ret.z = this->z + other.z;
			return ret;
		}

		/// <summary>
		/// 足し算
		/// </summary>
		/// <param name="other">足すベクトル</param>
		/// <returns>自身に引数のベクトルを足した結果ベクトル</returns>
		Vec3 operator+=(const Vec3& other)
		{
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;
		}

		/// <summary>
		/// 引き算
		/// </summary>
		/// <param name="other">引くベクトル</param>
		/// <returns>自身から引数のベクトルを引いた結果ベクトル</returns>
		Vec3 operator-(const Vec3& other)const
		{
			Vec3 ret;
			ret.x = this->x - other.x;
			ret.y = this->y - other.y;
			ret.z = this->z - other.z;
			return ret;
		}

		/// <summary>
		/// 引き算
		/// </summary>
		/// <param name="other">引くベクトル</param>
		/// <returns>自身から引数のベクトルを引いた結果ベクトル</returns>
		Vec3 operator-=(const Vec3& other)
		{
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;
		}

		/// <summary>
		/// 掛け算
		/// </summary>
		/// <param name="scale">かける数</param>
		/// <returns>自身のそれぞれにかける数を掛けた結果ベクトル</returns>
		Vec3 operator*(const float scale)
		{
			Vec3 ret;
			ret.x = this->x * scale;
			ret.y = this->y * scale;
			ret.z = this->z * scale;
			return ret;
		}

		/// <summary>
		/// 代入
		/// </summary>
		/// <param name="vec">代入するベクトル</param>
		/// <returns>代入結果ベクトル</returns>
		Vec3 operator=(const VECTOR& vec)
		{
			Vec3 ret;
			ret.x = vec.x;
			ret.y = vec.y;
			ret.z = vec.z;
			return ret;
		}

		/// <summary>
		/// 比較
		/// </summary>
		/// <param name="other">比較対象ベクトル</param>
		/// <returns>比較結果　false : 同一でない, true : 同一</returns>
		bool operator==(const Vec3& other)const
		{
			bool ret;
			ret = this->x == other.x && this->y == other.y && this->z == other.z;
			return ret;
		}

		/// <summary>
		/// 比較
		/// </summary>
		/// <param name="other">比較対象ベクトル</param>
		/// <returns>比較結果　false : 同一, true : 同一でない</returns>
		bool operator!=(const Vec3& other)const
		{
			bool ret;
			ret = this->x != other.x || this->y != other.y || this->z != other.z;
			return ret;
		}

		/// <summary>
		/// ベクトルのサイズを取得する
		/// </summary>
		/// <returns>ベクトルのサイズ</returns>
		float Size()const
		{
			float ret;
			ret = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
			return ret;
		}

		/// <summary>
		/// ベクトルのサイズの2乗を取得する
		/// </summary>
		/// <returns>ベクトルのサイズの2乗</returns>
		float SquareSize()const
		{
			float ret;
			ret = this->x * this->x + this->y * this->y + this->z * this->z;
			return ret;
		}

		/// <summary>
		/// 正規化ベクトルを取得する
		/// </summary>
		/// <returns>正規化ベクトル</returns>
		Vec3 Normalize()const
		{
			Vec3 ret;
			float size = this->Size();
			if (size == 0.0f)
			{
				return ret;
			}
			ret.x = this->x / size;
			ret.y = this->y / size;
			ret.z = this->z / size;
			return ret;
		}

		/// <summary>
		/// 二つのベクトルの内積を取得する
		/// </summary>
		/// <param name="other">乗ベクトル</param>
		/// <returns>内積</returns>
		float Dot(const Vec3& other)const
		{
			float ret;
			ret = this->x * other.x + this->y * other.y + this->z * other.z;
			return ret;
		}

		/// <summary>
		/// 二つのベクトルの外積を取得する
		/// </summary>
		/// <param name="other">乗ベクトル</param>
		/// <returns>外積</returns>
		Vec3 Cross(const Vec3& other)const
		{
			Vec3 ret;
			ret.x = this->y * other.z - this->z * other.y;
			ret.y = this->z * other.x - this->x * other.z;
			ret.z = this->x * other.y - this->y * other.x;
			return ret;
		}

		/// <summary>
		/// DxLibのVECTOR型に変換
		/// </summary>
		/// <returns>DxLib::VECTOR型ベクトル</returns>
		DxLib::VECTOR ConvertToVECTOR()
		{
			DxLib::VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
			ret.x = this->x;
			ret.y = this->y;
			ret.z = this->z;
			return ret;
		}
	};
}