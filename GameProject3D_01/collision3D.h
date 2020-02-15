#ifndef COLLISION3D_H_
#define COLLISION3D_H_

////////////////////////////////////////////////////////
//
//	球型コリジョン
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
//	球型コリジョンとボーンの名前のセットクラス
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
//	OBBコリジョンクラス
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

	// 指定軸番号の方向ベクトルを取得
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
	// 各軸方向の長さを取得
	Vector3 GetLen_W() { return m_fLength; }

	// 指定軸方向の長さを取得　0 : right / 1 : up / 2 : front
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
	// 位置を取得
	Vector3 GetPos_W() { return m_Pos; }

private:
	Vector3    m_Pos;					// 位置
	Vector3X3  m_NormaDirect;	// 方向ベクトル
	Vector3    m_fLength;				// 各軸方向の長さ
	XMFLOAT4		m_Color;			// 表示色

};





/////////////////////////////////////////////////////////////////
// ＜::: 3D衝突判定 :::＞
//	・球 と 球
//	・球 と 点
/////////////////////////////////////////////////////////////////
class CCollision3DJudge
{
public:
	// 球と球の衝突判定
	static bool Collision3D_Spher_Spher(CCollisionSphere* pSA, CCollisionSphere* pSB);

	// 球と点の衝突判定
	static bool Collision3D_Spher_Point(CCollisionSphere* pSA, XMFLOAT3* pPB);

	// 球とOBBの衝突判定
	static bool Collision3D_Sphere_OBB(CCollisionSphere* pSphere, CCollisionOBB& obb);

	// OBBとOBBの衝突判定
	static bool Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2);

	// 点とOBBの距離を求める
	static float LenOBBToPoint(CCollisionOBB& obb, Vector3* p);

private:
	// 分離軸に投影された軸成分から投影線分長を算出
	static float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

};

#endif // !COLLISION3D_H_

