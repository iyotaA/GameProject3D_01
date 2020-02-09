#ifndef USER_INTERFACE_MANAGER_H_
#define USER_INTERFACE_MANAGER_H_

#include "polygon.h"

class CUserInterfaceManager
{
public:

	// UIマネージャー初期化
	static void Init();

	// UI生成
	static void AddUI(CPolygon2D* pPolygon);

	// UI更新
	static void Update();

	// UI投影
	static void Draw();

	// UI破壊
	static bool DeleteUI(const unsigned int _id);

	// UI破壊
	static void DeleteAllUI();

	// インスタンスを取得
	static CUserInterfaceManager& GetInstance() {

		CUserInterfaceManager instance;
		return instance;
	}

private:

	// ＜::: 変数群 :::＞ //
	static std::vector<CPolygon2D*> m_pPolygon; // ポリゴンポインタ

	CUserInterfaceManager() = default;
	~CUserInterfaceManager() = default;

};

#endif // !USER_INTERFACE_MANAGER_H_
