#include "main.h"
#include <vector>
#include "polygon.h"
#include "user_interface_manager.h"
#include <algorithm>

std::vector<CPolygon2D*> CUserInterfaceManager::m_pPolygon[LAYER_MAX]; // �|���S���|�C���^
bool DelUI(CPolygon2D* poly) { return poly->Delete(); }	// delete �t���O����

// UI�}�l�[�W���[������
void CUserInterfaceManager::Init()
{

}

// UI����
void CUserInterfaceManager::AddUI(CPolygon2D* pPolygon, LAYER_UI layer)
{
	m_pPolygon[layer].push_back(pPolygon);
}

// UI�X�V
void CUserInterfaceManager::Update()
{
	// delete �t���O�� true �ō폜
	for (int i = 0; i < LAYER_MAX; i++) {
		m_pPolygon[i].erase(
			std::remove_if(m_pPolygon[i].begin(), m_pPolygon[i].end(), &DelUI),
			m_pPolygon[i].end()
		);
	}
}

// UI���e
void CUserInterfaceManager::Draw()
{
	for (int i = 0; i < LAYER_MAX; i++) {

		for (CPolygon2D* polygon : m_pPolygon[i]) {

			polygon->Draw();
		}
	}
}

// UI�j��
bool CUserInterfaceManager::DeleteUI(const unsigned int _id)
{
	return true;
}

// �S�Ă�UI�j��
void CUserInterfaceManager::DeleteAllUI()
{
	for (int i = 0; i < LAYER_MAX; i++) {
		m_pPolygon[i].clear();
	}
}


// �w�背�C���[�ȊO��UI�j��
void CUserInterfaceManager::DeleteExpectUI(LAYER_UI layer)
{
	for (int i = 0; i < LAYER_MAX; i++) {

		if (i == layer)continue;

		m_pPolygon[i].clear();
	}
}
