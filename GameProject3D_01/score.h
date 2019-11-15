#ifndef SCORE_H_
#define SCORE_H_

class CScore
{
private:
	static int m_Score;
	static CNumber* m_Num;

public:
	static void Init()
	{
		m_Num = new CNumber;
		m_Num->Init("asset/number.tga");
		m_Num->SetPosition(XMFLOAT2(550.0f, 100.0f));

		m_Score = 0;
	}

	static void Uninit()
	{
		m_Num->Uninit();
		delete m_Num;
	}

	static void Draw()
	{
		m_Num->Draw();
	}

	static void AddScore()
	{
		m_Score++;
	}

	static void SetScore()
	{
		m_Num->SetNum((float)m_Score);
	}

};

#endif

