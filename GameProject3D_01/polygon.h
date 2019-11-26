#ifndef POLYGON_H_
#define POLYGON_H_

class CPolygon : public CGameObject
{
public:

	void Init(XMFLOAT2 position, XMFLOAT2 W_H, char* pFileName);
	void Uninit();
	void Update();
	void Draw();

protected:
	ID3D11Buffer*	m_VertexBufer = NULL;
	CTexture*		m_Texture = NULL;

};

#endif
