#ifndef COLLISION3D_H_
#define COLLISION3D_H_


class CCollisionSphere
{
private:
	XMFLOAT3	m_CenterPos;
	float		m_Radius;

public:
	CCollisionSphere() {
		m_CenterPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_Radius = 1.0f;
	}
	void SetCenter(XMFLOAT3* center) { m_CenterPos = *center; }
	void SetRadius(float radius) { m_Radius = radius; }
	XMFLOAT3 GetCenter(void) { return m_CenterPos; }
	float GetRadius(void) { return m_Radius; }
};


class CJudgeCollisionSphere
{
public:
	static bool Collision3D_EachSpher(CCollisionSphere* pSA, CCollisionSphere* pSB)
	{
		// ‹…A‚Æ‹…B‚Ì·•ª‚ğ‹‚ß‚é
		XMFLOAT3 sub;
		sub.x = pSB->GetCenter().x - pSA->GetCenter().x;
		sub.y = pSB->GetCenter().y - pSA->GetCenter().y;
		sub.z = pSB->GetCenter().z - pSA->GetCenter().z;

		return sqrtf(sub.x * sub.x + sub.y * sub.y + sub.z * sub.z) <= pSB->GetRadius() + pSA->GetRadius() ? true : false;

	}
};

#endif // !COLLISION3D_H_

