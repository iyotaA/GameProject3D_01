#ifndef POLYGON3D_H_
#define POLYGON3D_H_

class CPolygon3D : public CGameObject
{
private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	CTexture*		m_Texture = NULL;
	float			m_Counter = 0.0f;

public:
	void Init(XMFLOAT3 pos, XMFLOAT3 scal, XMFLOAT3 rot);
	void Uninit();
	void Update();
	void Draw();
};

#endif
