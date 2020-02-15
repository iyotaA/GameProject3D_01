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


class CTexture;
class CShader2D;

class CPolygon2D
{
public:
	CPolygon2D(char* pFileName);
	~CPolygon2D();

	virtual void Draw() = 0;

	void SetPosition(XMFLOAT2 position) { m_Position = position; }
	void SetSize(XMFLOAT2 size) { m_Size = size; }// pixels
	void SetUV(UV uv) { m_UV = uv; }
	void SetColor(XMFLOAT4 color) { m_Color = color; }
	void SetRotate(float deg) { m_RotateDeg = deg; }
	bool& Delete() { return m_Delete; }

	XMFLOAT2& Size() { return m_Size; }

protected:

	ID3D11Buffer*	m_VertexBufer = NULL;
	CTexture*		m_Texture = NULL;
	CShader2D*		m_Shader = NULL;

	XMFLOAT2	m_Position;
	UV			m_UV;
	XMFLOAT2	m_Size;
	XMFLOAT4	m_Color;
	float		m_RotateDeg;
	bool		m_Delete;

private:
	CPolygon2D(){}

};

#endif

