#ifndef SKYDOME_H_
#define SKYDOME_H_

class CSkyDome : public CGameObject
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	ID3D11Buffer*	m_IndexBufer = NULL;
	CTexture*		m_Texture = NULL;
	CShaderDefault* m_Shader;
	int				m_IndexNum;

};

#endif
