#include "main.h"
#include <vector>
#include "polygon.h"
#include "user_interface_manager.h"
#include <algorithm>

std::vector<CPolygon2D*> CUserInterfaceManager::m_pPolygon[LAYER_MAX]; // ポリゴンポインタ
bool DelUI(CPolygon2D* poly) { return poly->Delete(); }	// delete フラグ判定

// UIマネージャー初期化
void CUserInterfaceManager::Init()
{

}

// UI生成
void CUserInterfaceManager::AddUI(CPolygon2D* pPolygon, LAYER_UI layer)
{
	m_pPolygon[layer].push_back(pPolygon);
}

// UI更新
void CUserInterfaceManager::Update()
{
	// delete フラグが true で削除
	for (int i = 0; i < LAYER_MAX; i++) {
		m_pPolygon[i].erase(
			std::remove_if(m_pPolygon[i].begin(), m_pPolygon[i].end(), &DelUI),
			m_pPolygon[i].end()
		);
	}
}

// UI投影
void CUserInterfaceManager::Draw()
{
	for (int i = 0; i < LAYER_MAX; i++) {

		for (CPolygon2D* polygon : m_pPolygon[i]) {

			polygon->Draw();
		}
	}
}

// UI破壊
bool CUserInterfaceManager::DeleteUI(const unsigned int _id)
{
	return true;
}

// 全てのUI破壊
void CUserInterfaceManager::DeleteAllUI()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		m_pPolygon[i].clear();
	}
}


// 指定レイヤー以外のUI破壊
void CUserInterfaceManager::DeleteExpectUI(LAYER_UI layer)
{
	for (int i = 0; i < LAYER_MAX; i++) {

		if (i == layer)continue;

		m_pPolygon[i].clear();
	}
}
