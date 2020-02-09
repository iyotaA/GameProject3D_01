#ifndef USER_INTERFACE_MANAGER_H_
#define USER_INTERFACE_MANAGER_H_

#include "polygon.h"

class CUserInterfaceManager
{
public:

	// UI�}�l�[�W���[������
	static void Init();

	// UI����
	static void AddUI(CPolygon2D* pPolygon);

	// UI�X�V
	static void Update();

	// UI���e
	static void Draw();

	// UI�j��
	static bool DeleteUI(const unsigned int _id);

	// UI�j��
	static void DeleteAllUI();

	// �C���X�^���X���擾
	static CUserInterfaceManager& GetInstance() {

		CUserInterfaceManager instance;
		return instance;
	}

private:

	// ��::: �ϐ��Q :::�� //
	static std::vector<CPolygon2D*> m_pPolygon; // �|���S���|�C���^

	CUserInterfaceManager() = default;
	~CUserInterfaceManager() = default;

};

#endif // !USER_INTERFACE_MANAGER_H_
