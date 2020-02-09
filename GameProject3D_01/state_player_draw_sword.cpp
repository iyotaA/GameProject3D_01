#include "game_objects_all.h"
#include "state_player_idle.h"
#include "modelAnimation.h"
#include "player.h"
#include "state_player_draw_sword.h"
#include "state_player_attack_jump.h"

CStatePlayerDrawSword::CStatePlayerDrawSword(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0.0f)
	, m_IsMove(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_DRAW_A_SWORD, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerDrawSword::~CStatePlayerDrawSword()
{

}

void CStatePlayerDrawSword::Update(CPlayer* pPlayer)
{
	// カウンター更新
	m_FrameCounter++;

	if (m_FrameCounter == 40) {
		pPlayer->WeaponState() = SWORD_STATE_DRAW;
	}

	if (m_FrameCounter < pPlayer->GetCurrentAnimFrameNum() - 10) return;

	pPlayer->WeaponState() = SWORD_STATE_DRAW;
	if (m_IsMove) {

		// 攻撃ステートに遷移
		pPlayer->ChangeState(new CStatePlayerAttackJump(pPlayer, true));
	}

	// 待機ステートに遷移
	pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
}

