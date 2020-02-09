#include "game_objects_all.h"
#include "state_player_idle.h"
#include "state_player_move.h"
#include "state_player_move_run.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "state_player_damage.h"
#include "state_player_draw_sword.h"
#include "state_player_sheathe_sword.h"
#include "state_player_block.h"
#include "state_player_dodge.h"
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
	// �_���[�W�X�e�[�g�ɑJ��
	if (pPlayer->Damaged()) {
		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer));
		return;
	}

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

	// �J�E���^�[�X�V
	m_FrameCounter++;
}

bool CStatePlayerMove::Action(CPlayer* pPlayer)
{
	if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {

		// 10F�o���Ă������\��
		//if (m_FrameCounter >= 10) {
			pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
			return true;
		//}
	}

	// �U���X�e�[�g�ɑJ��
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		//// �[�����͔����X�e�[�g�ɑJ��
		//if ((pPlayer->WeaponState() == SWORD_STATE_SHEATHE)) {

		//	pPlayer->ChangeState(new CStatePlayerDrawSword(pPlayer, true));
		//	return true;
		//}
		pPlayer->WeaponState() = SWORD_STATE_DRAW;
		pPlayer->ChangeState(new CStatePlayerAttack(pPlayer, PLAYER_STATE_ATTACK_JUMP));
		return true;
	}

	// �h��X�e�[�g�ɑJ��
	if (CInput::GetGamepadPress(LEFT_TRRIGER) || CInput::GetKeyPress(VK_LSHIFT)) {

		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {
			pPlayer->ChangeState(new CStatePlayerBlock(pPlayer));
			return true;
		}
	}

	// �[���X�e�[�g�ɑJ��
	if ((pPlayer->WeaponState() == SWORD_STATE_DRAW) &&
		((CInput::GetGamepadTrigger(XINPUT_GAMEPAD_X) || CInput::GetKeyTrigger('K')))) {

		pPlayer->ChangeState(new CStatePlayerSheatheSword(pPlayer, true));
		return true;
	}

	return false;
}

void CStatePlayerMove::ChangeState(CStatePlayerMove* pMoveState)
{
	delete m_pStateMove;
	m_pStateMove = pMoveState;
}
