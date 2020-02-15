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

	// UIマネージャー初期化
	static void Init();

	// UI生成
	static void AddUI(CPolygon2D* pPolygon, LAYER_UI layer);

	// UI更新
	static void Update();

	// UI投影
	static void Draw();

	// UI破壊
	static bool DeleteUI(const unsigned int _id);

	// 全てのUI破壊
	static void DeleteAllUI();

	// 指定レイヤー以外のUI破壊
	static void DeleteExpectUI(LAYER_UI layer);

	// インスタンスを取得
	static CUserInterfaceManager& GetInstance() {

		CUserInterfaceManager instance;
		return instance;
	}

private:

	// ＜::: 変数群 :::＞ //
	static std::vector<CPolygon2D*> m_pPolygon[LAYER_MAX]; // ポリゴンポインタ

	CUserInterfaceManager() = default;
	~CUserInterfaceManager() = default;

};

#endif // !USER_INTERFACE_MANAGER_H_
