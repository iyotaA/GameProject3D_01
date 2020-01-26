#include "state_player_idle.h"
#include "state_player_move_run.h"
#include "game_objects_all.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerIdle::CStatePlayerIdle(CPlayer* pPlayer)
{
	pPlayer->SetAnimation(0, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerIdle::~CStatePlayerIdle()
{

}

void CStatePlayerIdle::Update(CPlayer* pPlayer)
{
	if (pPlayer->PressMovingEntry()) {
		pPlayer->ChangeState(new CStatePlayerMove(pPlayer));
	}
}
