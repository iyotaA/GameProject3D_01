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
	void Update(const XMFLOAT3 pos);
	void Draw();

	void SetPosition(const XMFLOAT3 pos) { m_Position = pos; }
	void SetRotation(const XMFLOAT3 rot) { m_Rotation = rot; }
	void SetScale(const XMFLOAT3 scale)	 { m_Scale	  = scale; }

};


#endif // !WALL_H_
