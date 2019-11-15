#ifndef NUMBER_H_
#define NUMBER_H_

class CNumber : public CGameObject
{
private:
	CTexture* m_Texture;
	float m_Num;

public:
	void Init(char* pFileName);
	void Update();
	void Uninit();
	void Draw();

	void SetPosition(XMFLOAT2 pos)
	{
		m_Position.x = pos.x;
		m_Position.y = pos.y;
	}

	void SetNum(float num)
	{
		m_Num = num;
	}
};

#endif // !NUMBER_H_
