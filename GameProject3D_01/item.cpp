#include "item.h"
#include "image.h"
#include "number.h"
#include "user_interface_manager.h"

CItem::CItem(int count)
	: m_Count(count)
{
}

CItem::~CItem() {
	delete m_Polygon;
	delete m_Number;
}

CImage* CItem::GetPolygon()
{
	return m_Polygon;
}
