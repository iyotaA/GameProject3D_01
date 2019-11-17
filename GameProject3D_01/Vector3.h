
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
	Vector3(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	//コンストラクタ
	Vector3(const XMVECTOR& other) :XMFLOAT3() {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}

	inline BOOL operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline BOOL operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
	inline Vector3 operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
	inline Vector3 operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

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
