#ifndef NUMBER_H_
#define NUMBER_H_

#include "polygon.h"

class CNumber : public CPolygon2D
{
public:
	CNumber(char* pFileName)
		: CPolygon2D(pFileName)
		, m_Num(0)
	{}
	~CNumber(){}

	void Draw()override;
	void SetNum(float num) { m_Num = num; }

private:
	void SetBuffer();
	void DrawBuffer();

	float m_Num;

	// íËêî
	const int   COUNTER_STOP = 9999;
	const int   MAX_DIGIT = 4;
	const float SPACE_MAGNIFICATION = 0.6f;

};

#endif // !NUMBER_H_
