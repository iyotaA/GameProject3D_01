#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move.h"
#include "state_player_move_run.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "modelAnimation.h"
#include "player.h"
#include "MathFunc.h"

CStatePlayerMove::CStatePlayerMove(CPlayer* pPlayer)
	: m_pStateMove(new CStatePlayerMoveRun(pPlayer, 1.0f))
{
}

CStatePlayerMove::~CStatePlayerMove()
{
	delete m_pStateMove;
}

void CStatePlayerMove::Update(CPlayer* pPlayer)
{
	// ���͂�����ĂȂ���Αҋ@���[�V������
	if (!MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// �A�N�V����
	if (Action(pPlayer)) {
		return;
	}

	// �X�V
	m_pStateMove->UpdateMoveState(this, pPlayer);
}

bool CStatePlayerMove::Action(CPlayer* pPlayer)
{
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y)) {
		pPlayer->ChangeState(new CStatePlayerAttack(pPlayer));
		return true;
	}

	return false;
}

void CStatePlayerMove::ChangeState(CStatePlayerMove* pMoveState)
{
	delete m_pStateMove;
	m_pStateMove = pMoveState;
}
