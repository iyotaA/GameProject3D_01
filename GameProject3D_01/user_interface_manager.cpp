#include "main.h"
#include <vector>
#include "polygon.h"
#include "user_interface_manager.h"

std::vector<CPolygon2D*> CUserInterfaceManager::m_pPolygon; // �|���S���|�C���^


// UI�}�l�[�W���[������
void CUserInterfaceManager::Init()
{

}

// UI����
void CUserInterfaceManager::AddUI(CPolygon2D* pPolygon)
{
	m_pPolygon.push_back(pPolygon);
}

// UI�X�V
void CUserInterfaceManager::Update()
{

}

// UI���e
void CUserInterfaceManager::Draw()
{
	for (CPolygon2D* polygon : m_pPolygon) {

		polygon->Draw();
	}
}

// UI�j��
bool CUserInterfaceManager::DeleteUI(const unsigned int _id)
{

}

// UI�j��
void CUserInterfaceManager::DeleteAllUI()
{
	m_pPolygon.clear();
}

