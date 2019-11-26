#ifndef COLLISION3D_H_
#define COLLISION3D_H_

// 球型コリジョン
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
	// 指定軸方向の長さを取得
	Vector3 GetLen_W() { return m_fLength; }
	// 位置を取得
	Vector3 GetPos_W() { return m_Pos; }

private:
	Vector3    m_Pos;              // 位置
	Vector3X3  m_NormaDirect;	   // 方向ベクトル
	Vector3    m_fLength;          // 各軸方向の長さ


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

	// OBBとOBBの衝突判定
	static bool Collision3D_OBB_OBB(CCollisionOBB& obb1, CCollisionOBB& obb2);

private:
	// 分離軸に投影された軸成分から投影線分長を算出
	static float LenSegOnSeparateAxis(Vector3* Sep, Vector3* e1, Vector3* e2, Vector3* e3 = 0);

};

#endif // !COLLISION3D_H_

