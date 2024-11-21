#include "LocalPos.h"

LocalPos::LocalPos():
	centerPos(),
	localPos(),
	centerRotVec()
{
}

Vec3 LocalPos::GetWorldPos()
{
	Vec3 ret;

	Vec3 move = localPos;

	MATRIX mat;
	CreateRotationXYZMatrix(&mat,centerRotVec.x, centerRotVec.y, centerRotVec.z);

	//move = move.MatTransform(mat);

	//ans = move + m_centerPos;

	return ret;
}

void LocalPos::SetFrontPos(Vec3 frontPos)
{
	float vX = centerPos.x - frontPos.x;
	float vZ = centerPos.z - frontPos.z;

	float yAngle = std::atan2f(vX, vZ);

	Vec3 rot(0.0f, yAngle + DX_PI_F, 0.0f);

	centerRotVec = rot;
}
