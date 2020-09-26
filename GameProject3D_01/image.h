#ifndef IMAGE_H_
#define IMAGE_H_

#include "polygon.h"
class CImage : public CPolygon2D
{
public:
	CImage(char* pFileName)
		: CPolygon2D(pFileName)
	{}
	~CImage() {}

	virtual void Draw()override;

private:
	CImage()
		: CPolygon2D("")
	{}
	void SetBuffer();
	void DrawBuffer();
};

#endif // !IMAGE_H_
