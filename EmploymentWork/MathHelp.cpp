#include "MathHelp.h"
//
//Vec3 GetNearestPtOnLine(const Vec3& start, const Vec3& end, const Vec3& point)
//{
//	Vec3 startToEnd = end - start;
//	Vec3 startToPoint = point - start;
//
//	// üã‚Ì‚Ç‚Ì•Ó‚©
//	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
//	// ”r‘¼ˆ—
//	t = std::fmax(std::fmin(t, 1.0f), 0.0f);
//
//	return start + startToEnd * t;
//}
