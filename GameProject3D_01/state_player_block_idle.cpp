#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_damage_large.h"
#include "state_player_damage_small.h"
#include "player.h"
#include "state_player_block_idle.h"
#include "state_player_block_react.h"

CStatePlayerBlockIdle::CStatePlayerBlockIdle(CPlayer* pPlayer)
{
	pPlayer->SetAnimation(PLAYER_STATE_BLOCK_IDLE, 1.0f);
	pPlayer->SetAnimationSpeed(2.5f);
}

CStatePlayerBlockIdle::~CStatePlayerBlockIdle()
{
}

void CStatePlayerBlockIdle::UpdateBlockState(CStatePlayerBlock* pStateBlock, CPlayer* pPlayer)
{
	// –hŒä—LŒø’†‚Í‘JˆÚ‚³‚¹‚È‚¢
	if (pStateBlock->FrameCountStart())return;

	// “G‚ÌUŒ‚‚ð–hŒäA–hŒäƒXƒe[ƒg‚É‘JˆÚ
	if (pPlayer->Damaged()) {
		pStateBlock->ChangeState(new CStatePlayerBlockReact(pStateBlock, pPlayer));
	}
}
