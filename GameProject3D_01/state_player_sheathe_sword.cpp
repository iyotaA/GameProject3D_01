#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move.h"
#include "modelAnimation.h"
#include "player.h"
#include "state_player_sheathe_sword.h"

CStatePlayerSheatheSword::CStatePlayerSheatheSword(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0)
	, m_IsMove(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_SHEATHE_A_SWORD, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerSheatheSword::~CStatePlayerSheatheSword()
{

}

void CStatePlayerSheatheSword::Update(CPlayer* pPlayer)
{
	// カウンター更新
	m_FrameCounter++;

	if (m_FrameCounter <= 30)return;

	pPlayer->WeaponState() = SWORD_STATE_SHEATHE;

	if (m_IsMove) {
		// 移動ステートに遷移
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
	}
	else {
		// 待機ステートに遷移
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
	}
}

