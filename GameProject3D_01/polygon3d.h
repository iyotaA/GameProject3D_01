#ifndef POLYGON3D_H_
#define POLYGON3D_H_

class CPolygon3D : public CGameObject
{
public:
	void Init(Vector3 pos, Vector3 scal, Vector3 rot);
	void Uninit();
	void Update();
	void Draw();

private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	CTexture*		m_Texture = NULL;
	float			m_Counter = 0.0f;

};

#endif
