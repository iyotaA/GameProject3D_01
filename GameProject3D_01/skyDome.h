#ifndef SKYDOME_H_
#define SKYDOME_H_

class CSkyDome : public CGameObject
{
private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	ID3D11Buffer*	m_IndexBufer = NULL;
	CTexture*		m_Texture = NULL;
	int				m_IndexNum;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};

#endif
