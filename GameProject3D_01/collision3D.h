#ifndef COLLISION3D_H_
#define COLLISION3D_H_

////////////////////////////////////////////////////////
//
//	���^�R���W����
//
////////////////////////////////////////////////////////
class CCollisionSphere
{
public:
	CCollisionSphere() : m_CenterPos(Vector3(0.0f, 0.0f, 0.0f)), m_Radius(1.0f), m_Color(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) {}
	CCollisionSphere(Vector3 _pos, float _radius, XMFLOAT4 _color)
		: m_CenterPos(_pos)
		, m_Radius(_radius)
		, m_Color(_color)
	{}

	void SetCenter(Vector3* center) { m_CenterPos = *center; }
	void SetColor(XMFLOAT4* color) { m_Color = *color; }
	void SetRadius(float radius) { m_Radius = radius; }
	Vector3 GetCenter(void) { return m_CenterPos; }
	XMFLOAT4 GetColor(void) { return m_Color; }
	float GetRadius(void) { return m_Radius; }

private:
	Vector3			m_CenterPos;
	XMFLOAT4		m_Color;
	float				m_Radius;

};

////////////////////////////////////////////////////////
//
//	���^�R���W�����ƃ{�[���̖��O�̃Z�b�g�N���X
//
////////////////////////////////////////////////////////
class CCollisionWithBone
{
public:
	CCollisionWithBone(CCollisionSphere* pCollision_sphere, std::string name)
		: m_CollisionSphere(pCollision_sphere)
		, m_BoneName(name)
	{}

	CCollisionSphere* GetSphere() { return m_CollisionSphere; }
	const char* GetBoneName() { return m_BoneName.c_str(); }

private:
	CCollisionWithBone() {}

	CCollisionSphere* m_CollisionSphere;
	std::string		  m_BoneName;
};


////////////////////////////////////////////////////////
//
//	OBB�R���W�����N���X
//
////////////////////////////////////////////////////////
class CCollisionOBB
{
public:
	CCollisionOBB(Vector3 _pos, Vector3X3 _vec3, Vector3 _len, XMFLOAT4 _color)
		: m_Pos(_pos)
		, m_NormaDirect(_vec3)
		, m_fLength(_len)
		, m_Color(_color)
	{}

	CCollisionOBB() :
		m_Pos(),
		m_NormaDirect(),
		m_fLength()
		, m_Color()
	{}

	void SetStatus(Vector3* _pos, Vector3X3* _vec3, Vector3* _len, XMFLOAT4* _color)
	{
		m_Pos = *_pos;
		m_NormaDirect.front = _vec3->front;
		m_NormaDirect.right = _vec3->right;
		m_NormaDirect.up    = _vec3->up;
		m_fLength = *_len;
		m_Color = *_color;
	}

	void SetPosition(Vector3* _pos){ m_Pos = *_pos;	}
	void SetVec3Dir(Vector3X3* _vec3){ m_NormaDirect = *_vec3;	}
	void Set3Length(Vector3* _len){ m_fLength = *_len;	}
	void SetColor(XMFLOAT4* _color){ m_Color = *_color;	}

	XMFLOAT4 GetColor() { return m_Color; }

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
	// �e�������̒������擾
	Vector3 GetLen_W() { return m_fLength; }

	// �w�莲�����̒������擾�@0 : right / 1 : up / 2 : front
	float GetLen_W(int elem) {

		if (elem == 0)
			return m_fLength.x;
		else if (elem == 1)
			return m_fLength.y;
		else if (elem == 2)
			return m_fLength.z;
		else
			return 1.0f;
	}
	// �ʒu���擾
	Vector3 GetPos_W() { return m_Pos; }

private:
	Vector3    m_Pos;					// �ʒu
	Vector3X3  m_NormaDirect;	// �����x�N�g��
	Vector3    m_fLength;				// �e�������̒���
	XMFLOAT4		m_Color;			// �\���F

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

	// ����OBB�̏Փ˔���
	static bool Collision3D_Sphere_OBB(CCollisionSphere* pSphere, CCollisionOBB& obb);

	// OBB��OBB�̏Փ˔���
	static bool Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2);

	// �_��OBB�̋��������߂�
	static float LenOBBToPoint(CCollisionOBB& obb, Vector3* p);

private:
	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	static float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

};

#endif // !COLLISION3D_H_

