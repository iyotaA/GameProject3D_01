
using namespace DirectX;

#define XMVECX(v) v.m128_f32[0] //X���W�ɓ�����
#define XMVECY(v) v.m128_f32[1] //Y���W�ɓ�����
#define XMVECZ(v) v.m128_f32[2] //Z���W�ɓ�����
#define XMVECW(v) v.m128_f32[3] //W���W�ɓ�����(��{�I��1.0f�ł���)
#define XMVECDot(n, v) XMVectorGetX(XMVector3Dot(n, v)) //���ςɓ�����
#define XMVECCross(n, v) XMVectorGetX(XMVector3Cross(n, v)) //�O�ςɓ�����
#define XMVECLengthSq(d) XMVectorGetX(XMVector3LengthSq(d)) //�����ɓ�����
#define XMVECScale(n, v) XMVectorGetX(XMVector3Scale(n, v)) //�傫���ɓ�����


struct Vector3 : public XMFLOAT3 {

	Vector3() = default;
	Vector3(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}
	//�R���X�g���N�^
	Vector3(const XMVECTOR& other) :XMFLOAT3() {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
	}

	inline BOOL operator == (const Vector3& r) const { return x == r.x && y == r.y && z == r.z; }
	inline BOOL operator != (const Vector3& r) const { return x != r.x || y != r.y || z != r.z; }
	inline Vector3 operator *(const float r) const { return Vector3(x * r, y * r, z * r); }
	inline Vector3 operator /(const float r) const { return Vector3(x / r, y / r, z / r); }

	// �x�N�g���̓���
	float VDot(Vector3 In) { return x * In.x + y * In.y + z * In.z; }

	// �x�N�g���̊O��
	Vector3 VCross(Vector3 In) { return Vector3(y * In.z - z * In.y, z * In.x - x * In.z, x * In.y - y * In.x); }

	// �x�N�g���̃X�P�[�����O
	Vector3 VScale(float Scale) { Vector3 Result = { x * Scale, y * Scale, z * Scale }; return Result; }

	//���
	Vector3& operator=(const XMVECTOR& other) {
		XMVECTOR temp = other;
		XMStoreFloat3(this, temp);
		return *this;
	}
	//�L���X�g
	operator XMVECTOR() const {
		return XMLoadFloat3(this);
	}
	//����
	float Length() const {
		return (static_cast<Vector3>(XMVector3Length(XMVECTOR(*this)))).x;
	}
	//���K��
	void Normalize() {
		*this = XMVector3Normalize(XMVECTOR(*this));
	}
};
