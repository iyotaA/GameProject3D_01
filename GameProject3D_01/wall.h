#ifndef WALL_H_
#define WALL_H_

class CWall : public CGameObject
{
private:
	ID3D11Buffer*	m_VertexBufer = NULL;
	ID3D11Buffer*	m_IndexBufer = NULL;
	CTexture*		m_Texture = NULL;
	int				m_IndexNum;

public:
	void Init();
	void Uninit();
	void Update(const Vector3 pos);
	void Draw();

};


#endif // !WALL_H_
