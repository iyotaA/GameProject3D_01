#include "main.h"
#include "collision3D.h"

// 分離軸に投影された軸成分から投影線分長を算出
float CCollision3DJudge::LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3)
{
	XMVECTOR SEP, E1, E2, E3;

	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabs(Sep->VDot(*e1));
	float r2 = fabs(Sep->VDot(*e2));
	float r3 = e3 ? (fabs(Sep->VDot(*e3))) : 0;
	return r1 + r2 + r3;
}
// 球と球の衝突判定
bool CCollision3DJudge::Collision3D_Spher_Spher(CCollisionSphere* pSA, CCollisionSphere* pSB)
{
	// 球Aと球Bの差分を求める
	XMFLOAT3 sub;
	sub.x = pSB->GetCenter().x - pSA->GetCenter().x;
	sub.y = pSB->GetCenter().y - pSA->GetCenter().y;
	sub.z = pSB->GetCenter().z - pSA->GetCenter().z;

	return sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z) <= pSB->GetRadius() + pSA->GetRadius() ? true : false;

}

// 球と点の衝突判定
bool CCollision3DJudge::Collision3D_Spher_Point(CCollisionSphere* pSA, XMFLOAT3* pPB)
{
	// 球Aと球Bの差分を求める
	XMFLOAT3 sub;
	sub.x = pPB->x - pSA->GetCenter().x;
	sub.y = pPB->y - pSA->GetCenter().y;
	sub.z = pPB->z - pSA->GetCenter().z;

	return sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z) <= pSA->GetRadius() ? true : false;
}

bool CCollision3DJudge::Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2)
{
	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	Vector3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W().x;
	Vector3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W().y;
	Vector3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W().z;
	Vector3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W().x;
	Vector3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W().y;
	Vector3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W().z;
	Vector3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// 分離軸 : Ae1
	float rA = Ae1.Length();
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(Interval.VDot(NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Interval.VDot(NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Interval.VDot(NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.Length();
	L = fabs(Interval.VDot(NBe1));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.Length();
	L = fabs(Interval.VDot(NBe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.Length();
	L = fabs(Interval.VDot(NBe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : C11
	Vector3 Cross;
	Cross = NAe1.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = NAe1.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = NAe1.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = NAe2.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = NAe2.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = NAe2.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = NAe3.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = NAe3.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = NAe3.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// 分離平面が存在しないので「衝突している」
	return true;
}

