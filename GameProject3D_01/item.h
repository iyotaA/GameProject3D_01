#ifndef ITEM_H_
#define ITEM_H_

#include "main.h"
#include "Vector3.h"

class CImage;
class CNumber;
class CItem
{
public:
	CItem(int count);
	virtual ~CItem();

	virtual void Use() = 0;

	CImage* GetPolygon();

protected:
	CImage* m_Polygon;
	CNumber* m_Number;
	int			  m_Count;

private:
	CItem() {}

};

#endif // !ITEM_H_
