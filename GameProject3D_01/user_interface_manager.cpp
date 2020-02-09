#include "main.h"
#include <vector>
#include "polygon.h"
#include "user_interface_manager.h"

std::vector<CPolygon2D*> CUserInterfaceManager::m_pPolygon; // ポリゴンポインタ


// UIマネージャー初期化
void CUserInterfaceManager::Init()
{

}

// UI生成
void CUserInterfaceManager::AddUI(CPolygon2D* pPolygon)
{
	m_pPolygon.push_back(pPolygon);
}

// UI更新
void CUserInterfaceManager::Update()
{

}

// UI投影
void CUserInterfaceManager::Draw()
{
	for (CPolygon2D* polygon : m_pPolygon) {

		polygon->Draw();
	}
}

// UI破壊
bool CUserInterfaceManager::DeleteUI(const unsigned int _id)
{

}

// UI破壊
void CUserInterfaceManager::DeleteAllUI()
{
	m_pPolygon.clear();
}

