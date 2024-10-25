#include "MathHelp.h"

/// <summary>
/// 線分と点の線分上の最近接点を取得
/// </summary>
Vec3 GetNearestPtOnLine(const Vec3& start, const Vec3& end, const Vec3& point)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// 線上のどの辺か
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
	// 排他処理
	t = std::fmax(std::fmin(t, 1.0f), 0.0f);

	return start + startToEnd * t;
}

/// <summary>
/// 線分と線分のそれぞれの線分上の最近接点を取得
/// </summary>
void GetNearestPtOnLine(const Vec3& centerA, const Vec3& sizeA, const Vec3& centerB, const Vec3& sizeB, Vec3& resultAPos, Vec3& resultBPos)
{
	// 相対ベクトル
	Vec3 vec = centerB - centerA;

	float s, t;

	s = Dot(sizeA, vec) / sizeA.SqLength();
	t = Dot(sizeB, -vec) / sizeB.SqLength();

	// 範囲の制限
	s = std::min<float>(std::max<float>(s, -1.0f), 1.0f);
	t = std::min<float>(std::max<float>(t, -1.0f), 1.0f);

	resultAPos = sizeA * s + centerA;
	resultBPos = sizeB * t + centerB;
}