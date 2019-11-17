#ifndef COLLISION3D_H_
#define COLLISION3D_H_

// ^RW
class CCollisionSphere
{
private:
	Vector3	m_CenterPos;
	float		m_Radius;

public:
	CCollisionSphere() : m_CenterPos(Vector3(0.0f, 0.0f, 0.0f)), m_Radius(1.0f){}

	void SetCenter(Vector3* center) { m_CenterPos = *center; }
	void SetRadius(float radius) { m_Radius = radius; }
	Vector3 GetCenter(void) { return m_CenterPos; }
	float GetRadius(void) { return m_Radius; }
};

// CCollisionOBB(Vector3 _pos, Dir3Vector _vec3, Vector3 _len)
class CCollisionOBB
{
private:
	Vector3    m_Pos;              // Êu
	Dir3Vector m_NormaDirect;   // ûüxNg
	Vector3    m_fLength;          // e²ûüÌ·³

public:
	CCollisionOBB(Vector3 _pos, Dir3Vector _vec3, Vector3 _len):
		m_Pos(_pos),
		m_NormaDirect(_vec3),
		m_fLength(_len){}

	CCollisionOBB() :
		m_Pos(),
		m_NormaDirect(),
		m_fLength() {}

	void SetStatus(Vector3* _pos, Dir3Vector* _vec3, Vector3* _len)
	{
		m_Pos = *_pos;
		m_NormaDirect.front = _vec3->front;
		m_NormaDirect.right = _vec3->right;
		m_NormaDirect.up    = _vec3->up;
		m_fLength = *_len;
	}

	// wè²ÔÌûüxNgðæ¾
	Vector3 GetDirect(int elem) {

		if (elem == 0)
			return m_NormaDirect.front;
		else if (elem == 1)
			return m_NormaDirect.right;
		else if (elem == 2)
			return m_NormaDirect.up;
		else
			assert(false);
			return Vector3(0.0f, 0.0f, 0.0f);
	}
	// wè²ûüÌ·³ðæ¾
	Vector3 GetLen_W() { return m_fLength; }
	// Êuðæ¾
	Vector3 GetPos_W() { return m_Pos; }
};



/////////////////////////////////////////////////////////////////
// ::: 3DÕË»è :::
//	E Æ 
//	E Æ _
/////////////////////////////////////////////////////////////////
class CJudgeCollision3D
{
private:
	// ª£²Ée³ê½²¬ª©çeüª·ðZo
	static float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

public:
	// ÆÌÕË»è
	static bool Collision3D_Spher_Spher(CCollisionSphere* pSA, CCollisionSphere* pSB);

	// Æ_ÌÕË»è
	static bool Collision3D_Spher_Point(CCollisionSphere* pSA, XMFLOAT3* pPB);

	// OBBÆOBBÌÕË»è
	static bool Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2);
};

#endif // !COLLISION3D_H_

