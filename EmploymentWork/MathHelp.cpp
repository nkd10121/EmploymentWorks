#include "MathHelp.h"
//
//Vec3 GetNearestPtOnLine(const Vec3& start, const Vec3& end, const Vec3& point)
//{
//	Vec3 startToEnd = end - start;
//	Vec3 startToPoint = point - start;
//
//	// ����̂ǂ̕ӂ�
//	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
//	// �r������
//	t = std::fmax(std::fmin(t, 1.0f), 0.0f);
//
//	return start + startToEnd * t;
//}
