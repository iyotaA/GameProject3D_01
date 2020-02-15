#ifndef USER_INTERFACE_MANAGER_H_
#define USER_INTERFACE_MANAGER_H_

#include "polygon.h"

class CUserInterfaceManager
{
public:
	typedef enum
	{
		LAYER_0,
		LAYER_1,
		LAYER_2,
		LAYER_FOREGROUND,
		LAYER_MAX
	}LAYER_UI;

	// UI�}�l�[�W���[������
	static void Init();

	// UI����
	static void AddUI(CPolygon2D* pPolygon, LAYER_UI layer);

	// UI�X�V
	static void Update();

	// UI���e
	static void Draw();

	// UI�j��
	static bool DeleteUI(const unsigned int _id);

	// �S�Ă�UI�j��
	static void DeleteAllUI();

	// �w�背�C���[�ȊO��UI�j��
	static void DeleteExpectUI(LAYER_UI layer);

	// �C���X�^���X���擾
	static CUserInterfaceManager& GetInstance() {

		CUserInterfaceManager instance;
		return instance;
	}

private:

	// ��::: �ϐ��Q :::�� //
	static std::vector<CPolygon2D*> m_pPolygon[LAYER_MAX]; // �|���S���|�C���^

	CUserInterfaceManager() = default;
	~CUserInterfaceManager() = default;

};

#endif // !USER_INTERFACE_MANAGER_H_
