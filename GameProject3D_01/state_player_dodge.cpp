#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerDodge::CStatePlayerDodge(CPlayer* pPlayer)
	:m_FrameCounter(0)
{
	pPlayer->SetAnimation(1, 1.0f);
	pPlayer->SetAnimationSpeed(1.5f);
	pPlayer->SetMoveSpeed(1.0f);
}

CStatePlayerDodge::~CStatePlayerDodge()
{
}

void CStatePlayerDodge::Update(CPlayer* pPlayer)
{
	if (pPlayer->AnimationBlending()) return;
	if (!pPlayer->CurrentAnimationFinish()) return;

	if (pPlayer->PressMovingEntry()) {
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
	}
	else {
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
	}
}

