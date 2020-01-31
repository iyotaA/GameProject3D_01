#ifndef NUMBER_H_
#define NUMBER_H_

class CNumber : public CGameObject
{
public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	void SetPosition(XMFLOAT2 pos) {
		m_Position = Vector3(pos.x, pos.y, 0.0f);
	}

	void SetNum(float num) { m_Num = num; }

private:
	float m_Num;
	CPolygon2D* m_Polygon;

	// íËêî
	const int   COUNTER_STOP = 9999;
	const int   MAX_DIGIT = 4;
	const float SPACE_MAGNIFICATION = 0.7f;

};

#endif // !NUMBER_H_
