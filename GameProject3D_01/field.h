#ifndef FIELD_H_
#define FIELD_H_

class CField : public CGameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	float GetHeight(XMFLOAT3* _position);

private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	ID3D11Buffer*	m_IndexBufer = NULL;
	CTexture*		m_Texture = NULL;
	VERTEX_3D*		m_Vertex;
	int				m_IndexNum;

};

#endif