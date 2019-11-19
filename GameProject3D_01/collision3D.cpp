#include "main.h"
#include "collision3D.h"

// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float CCollision3DJudge::LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3)
{
	XMVECTOR SEP, E1, E2, E3;

	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = fabs(Sep->VDot(*e1));
	float r2 = fabs(Sep->VDot(*e2));
	float r3 = e3 ? (fabs(Sep->VDot(*e3))) : 0;
	return r1 + r2 + r3;
}
// ���Ƌ��̏Փ˔���
bool CCollision3DJudge::Collision3D_Spher_Spher(CCollisionSphere* pSA, CCollisionSphere* pSB)
{
	// ��A�Ƌ�B�̍��������߂�
	XMFLOAT3 sub;
	sub.x = pSB->GetCenter().x - pSA->GetCenter().x;
	sub.y = pSB->GetCenter().y - pSA->GetCenter().y;
	sub.z = pSB->GetCenter().z - pSA->GetCenter().z;

	return sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z) <= pSB->GetRadius() + pSA->GetRadius() ? true : false;

}

// ���Ɠ_�̏Փ˔���
bool CCollision3DJudge::Collision3D_Spher_Point(CCollisionSphere* pSA, XMFLOAT3* pPB)
{
	// ��A�Ƌ�B�̍��������߂�
	XMFLOAT3 sub;
	sub.x = pPB->x - pSA->GetCenter().x;
	sub.y = pPB->y - pSA->GetCenter().y;
	sub.z = pPB->z - pSA->GetCenter().z;

	return sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z) <= pSA->GetRadius() ? true : false;
}

bool CCollision3DJudge::Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2)
{
	// �e�����x�N�g���̊m��
	// �iN***:�W���������x�N�g���j
	Vector3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W().x;
	Vector3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W().y;
	Vector3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W().z;
	Vector3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W().x;
	Vector3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W().y;
	Vector3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W().z;
	Vector3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// ������ : Ae1
	float rA = Ae1.Length();
	float rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	float L = fabs(Interval.VDot(NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = fabs(Interval.VDot(NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = fabs(Interval.VDot(NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = Be1.Length();
	L = fabs(Interval.VDot(NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = Be2.Length();
	L = fabs(Interval.VDot(NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = Be3.Length();
	L = fabs(Interval.VDot(NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	Vector3 Cross;
	Cross = NAe1.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = NAe1.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = NAe1.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = NAe2.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = NAe2.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = NAe2.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = NAe3.VCross(NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = NAe3.VCross(NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = NAe3.VCross(NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = fabs(Interval.VDot(Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}

