#pragma once
#include "Vec3.h"


/// <summary>
/// �����Ɠ_�̒�����̍ŋߐړ_���擾
/// </summary>
/// <param name="start">�����̎n�_</param>
/// <param name="end">�����̏I�_</param>
/// <param name="point">�_�̍��W</param>
/// <returns>������̍ŋߐړ_</returns>
Vec3 GetNearestPtOnLine(const Vec3& start, const Vec3& end, const Vec3& point);

/// <summary>
/// �����Ɛ����̂��ꂼ��̐�����̍ŋߐړ_���擾
/// </summary>
/// <param name="centerA">����A�̒��S���W</param>
/// <param name="sizeA">����A�̒����̔���</param>
/// <param name="centerB">����B�̒��S���W</param>
/// <param name="sizeB">����B�̒����̔���</param>
/// <param name="resultAPos">����A��̐���B�Ƃ̍ŋߐړ_���W</param>
/// <param name="resultBPos">����B��̐���A�Ƃ̍ŋߐړ_���W</param>
void GetNearestPtOnLine(const Vec3& centerA, const Vec3& sizeA, const Vec3& centerB, const Vec3& sizeB,Vec3& resultAPos,Vec3& resultBPos);
