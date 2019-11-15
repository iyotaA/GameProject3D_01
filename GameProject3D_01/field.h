#ifndef FIELD_H_
#define FIELD_H_

class CField : public CGameObject
{
private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	ID3D11Buffer*	m_IndexBufer = NULL;
	CTexture*		m_Texture = NULL;
	VERTEX_3D*		m_Vertex;
	int				m_IndexNum;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(XMFLOAT3* _position);
};

#endif