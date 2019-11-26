#ifndef COLLISION3D_H_
#define COLLISION3D_H_

// ���^�R���W����
class CCollisionSphere
{
public:
	CCollisionSphere() : m_CenterPos(Vector3(0.0f, 0.0f, 0.0f)), m_Radius(1.0f){}
	CCollisionSphere(Vector3 _pos, float _radius) : m_CenterPos(_pos), m_Radius(_radius){}

	void SetCenter(Vector3* center) { m_CenterPos = *center; }
	void SetRadius(float radius) { m_Radius = radius; }
	Vector3 GetCenter(void) { return m_CenterPos; }
	float GetRadius(void) { return m_Radius; }

private:
	Vector3	m_CenterPos;
	float	m_Radius;

};

// CCollisionOBB(Vector3 _pos, Dir3Vector _vec3, Vector3 _len)
class CCollisionOBB
{
public:
	CCollisionOBB(Vector3 _pos, Vector3X3 _vec3, Vector3 _len):
		m_Pos(_pos),
		m_NormaDirect(_vec3),
		m_fLength(_len){}

	CCollisionOBB() :
		m_Pos(),
		m_NormaDirect(),
		m_fLength() {}

	void SetStatus(Vector3* _pos, Vector3X3* _vec3, Vector3* _len)
	{
		m_Pos = *_pos;
		m_NormaDirect.front = _vec3->front;
		m_NormaDirect.right = _vec3->right;
		m_NormaDirect.up    = _vec3->up;
		m_fLength = *_len;
	}

	// �w�莲�ԍ��̕����x�N�g�����擾
	// 0 : right / 1 : up / 2 : front
	Vector3 GetDirect(int elem) {

		if (elem == 0)
			return m_NormaDirect.right;
		else if (elem == 1)
			return m_NormaDirect.up;
		else if (elem == 2)
			return m_NormaDirect.front;
		else
			assert(false);
			return Vector3(0.0f, 0.0f, 0.0f);
	}
	// �w�莲�����̒������擾
	Vector3 GetLen_W() { return m_fLength; }
	// �ʒu���擾
	Vector3 GetPos_W() { return m_Pos; }

private:
	Vector3    m_Pos;              // �ʒu
	Vector3X3  m_NormaDirect;	   // �����x�N�g��
	Vector3    m_fLength;          // �e�������̒���


};



/////////////////////////////////////////////////////////////////
// ��::: 3D�Փ˔��� :::��
//	�E�� �� ��
//	�E�� �� �_
/////////////////////////////////////////////////////////////////
class CCollision3DJudge
{
public:
	// ���Ƌ��̏Փ˔���
	static bool Collision3D_Spher_Spher(CCollisionSphere* pSA, CCollisionSphere* pSB);

	// ���Ɠ_�̏Փ˔���
	static bool Collision3D_Spher_Point(CCollisionSphere* pSA, XMFLOAT3* pPB);

	// OBB��OBB�̏Փ˔���
	static bool Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2);

private:
	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	static float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

};

#endif // !COLLISION3D_H_

