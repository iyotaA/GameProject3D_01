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
	// �h��L�����͑J�ڂ����Ȃ�
	if (pStateBlock->FrameCountStart())return;

	// �G�̍U����h��A�h��X�e�[�g�ɑJ��
	if (pPlayer->Damaged()) {
		pStateBlock->ChangeState(new CStatePlayerBlockReact(pStateBlock, pPlayer));
	}
}
