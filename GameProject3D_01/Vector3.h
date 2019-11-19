#ifndef VECTOR3_H_
#define VECTOR3_H_


using namespace DirectX;

#define XMVECX(v) v.m128_f32[0] //X座標に当たる
#define XMVECY(v) v.m128_f32[1] //Y座標に当たる
#define XMVECZ(v) v.m128_f32[2] //Z座標に当たる
#define XMVECW(v) v.m128_f32[3] //W座標に当たる(基本的に1.0fである)
#define XMVECDot(n, v) XMVectorGetX(XMVector3Dot(n, v)) //内積に当たる
#define XMVECCross(n, v) XMVectorGetX(XMVector3Cross(n, v)) //外積に当たる
#define XMVECLengthSq(d) XMVectorGetX(XMVector3LengthSq(d)) //長さに当たる
#define XMVECScale(n, v) XMVectorGetX(XMVector3Scale(n, v)) //大きさに当たる


struct Vector3 : public XMFLOAT3 {

	Vector3() = default;
	Vector3(float _x, float _y, float _z)
	{
		this->x = _x; this->y = _y; this->z = _z;
	}
	//コンストラクタ
	Vector3(const XMVECTOR& _other) :XMFLOAT3() {
		XMVECTOR temp = _other;
		XMStoreFloat3(this, temp);
	}

	inline BOOL    operator == (const Vector3& _r) const { return x == _r.x && y == _r.y && z == _r.z; }
	inline BOOL    operator != (const Vector3& _r) const { return x != _r.x || y != _r.y || z != _r.z; }
	inline Vector3 operator *  (const float _r) const { return Vector3(x * _r, y * _r, z * _r); }
	inline Vector3 operator /  (const float _r) const { return Vector3(x / _r, y / _r, z / _r); }
	inline Vector3 operator +  (const Vector3 _r) const { return Vector3(x + _r.x, y + _r.y, z + _r.z); }
	inline Vector3 operator -  (const Vector3 _r) const { return Vector3(x - _r.x, y - _r.y, z - _r.z); }

	inline void	   operator *= (const float _r) { this->x *= _r; this->y *= _r; this->z *= _r; }
	inline void	   operator /= (const float _r) { this->x /= _r; this->y /= _r; this->z /= _r; }
	inline void    operator += (const Vector3 _r) { this->x += _r.x; this->y += _r.y; this->z += _r.z;}
	inline void    operator -= (const Vector3 _r) { this->x -= _r.x; this->y -= _r.y; this->z -= _r.z;}
	inline void    operator *= (const Vector3 _r) { this->x *= _r.x; this->y *= _r.y; this->z *= _r.z;}
	inline void    operator /= (const Vector3 _r) { this->x /= _r.x; this->y /= _r.y; this->z /= _r.z;}

	// ベクトルの内積
	float VDot(Vector3 In) { return x * In.x + y * In.y + z * In.z; }

	// ベクトルの外積
	Vector3 VCross(Vector3 In) { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }

	// ベクトルのスケーリング
	Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

	//代入
	Vector3& operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}
	//キャスト
	operator XMVECTOR() const {
		return XMLoadFloat3(this);
	}
	//長さ
	float Length() const {
		return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
	}
	//正規化
	void Normalize() {
		*this = XMVector3Normalize(XMVECTOR(*this));
	}
};

struct Vector3X3
{
	Vector3 front, up, right;


	//コンストラクタ
	Vector3X3() = default;
	Vector3X3(Vector3 _right, Vector3 _up, Vector3 _front)
	{
		 right = _right; up = _up; front = _front;
	}

	inline Vector3X3 operator +  (const Vector3X3 r) const { return Vector3X3(front + r.front, up + r.up, front + r.front); }
	inline Vector3X3 operator -  (const Vector3X3 r) const { return Vector3X3(front - r.front, up - r.up, front - r.front); }

	inline void    operator += (const Vector3X3 r) { right += r.right; up += r.up; front += r.front;}
	inline void    operator -= (const Vector3X3 r) { right -= r.right; up -= r.up; front -= r.front;}
	inline void    operator *= (const Vector3X3 r) { right *= r.right; up *= r.up; front *= r.front;}
	inline void    operator /= (const Vector3X3 r) { right /= r.right; up /= r.up; front /= r.front;}

	//長さ
	float Length(int _id) const {
		switch (_id)
		{
		case 0:
			return right.Length();

		case 1:
			return up.Length();

		case 2:
			return front.Length();

		default:
			return -1;
		}
	}
	//正規化
	void NormalizeAll() {

		right.Normalize();
		up.Normalize();
		front.Normalize();
	}
};

#endif // !VECTOR3_H_