#ifndef POLYGON_H_
#define POLYGON_H_

struct UV
{
	UV() : su(0.0f), sv(0.0f), eu(1.0f), ev(1.0f) {}
	UV(float su, float sv, float eu, float ev) {
		this->su = su; this->sv = sv; this->eu = eu; this->ev = ev;
	}

	float su;
	float sv;
	float eu;
	float ev;
};

class CPolygon2D
{
public:
	CPolygon2D()
		: m_Position()
		, m_Size(XMFLOAT2(100.0f, 100.0f))
		, m_UV(UV())
		, m_Color(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
		, m_RotateDeg(0.0f)
	{}

	void Init(char* pFileName);
	void Uninit();
	void Draw();

	void SetPosition(XMFLOAT2 position) { m_Position = position; }
	void SetSize(XMFLOAT2 size) { m_Size = size; }// pixels
	void SetUV(UV uv) { m_UV = uv; }
	void SetColor(XMFLOAT4 color) { m_Color = color; }
	void SetRotate(float deg) { m_RotateDeg = deg; }

	XMFLOAT2& Size() { return m_Size; }

private:
	void SetBuffer();

private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	CTexture*		m_Texture = NULL;
	CShader2D*		m_Shader = NULL;

private:
	XMFLOAT2	m_Position;
	UV			m_UV;
	XMFLOAT2	m_Size;
	XMFLOAT4	m_Color;
	float		m_RotateDeg;

};

#endif
