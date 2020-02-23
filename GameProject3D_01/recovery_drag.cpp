#include "recovery_drag.h"
#include "user_interface_manager.h"
#include "game_objects_all.h"
#include "scene.h"
#include "item.h"
#include "image.h"
#include "number.h"

CRecoveryDrag::CRecoveryDrag(int count)
	: CItem(count)
{
	// 回復薬
	m_Polygon = new CImage("asset/image/user_interface/recovery_drag.png");
	m_Polygon->SetSize(XMFLOAT2(269 * 0.25, 388 * 0.25));
	m_Polygon->SetPosition(XMFLOAT2(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100));
	m_Polygon->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	m_Polygon->SetUV(UV(0.0f, 0.0f, 1.0f, 1.0f));

	CUserInterfaceManager::AddUI(m_Polygon, CUserInterfaceManager::LAYER_0);

	// Xボタン
	CImage* image = new CImage("asset/image/user_interface/button_X.png");
	image->SetSize(XMFLOAT2(256 * 0.12, 256 * 0.12));
	image->SetPosition(XMFLOAT2(SCREEN_WIDTH - 130, SCREEN_HEIGHT - 120));
	image->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	image->SetUV(UV(0.0f, 0.0f, 1.0f, 1.0f));
	CUserInterfaceManager::AddUI(image, CUserInterfaceManager::LAYER_0);

	// 個数設定
	m_Number = new CNumber("asset/image/user_interface/number.png");
	m_Number->SetNum(m_Count);
	m_Number->SetSize(XMFLOAT2(25.0f, 25.0f));
	m_Number->SetPosition(XMFLOAT2(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 50));
	m_Number->SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	CUserInterfaceManager::AddUI(m_Number, CUserInterfaceManager::LAYER_0);

}


void CRecoveryDrag::Use()
{
	// 残量があれば使用
	if (m_Count == 0)return;

	// 効果音再生
	CSound::Play(SOUND_LABEL_SE_RECOVERY);

	// プレイヤーのライフ(40)回復
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);
	player->Life() = (player->Life() <= 60) ? player->Life() + 40 : 100;	// 回復量

	m_Number->SetNum(--m_Count);	// 個数を減らす
}
