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
	// ダメージステートに遷移
	if (pPlayer->Damaged()) {
		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer));
		return;
	}

	// 入力がされてなければ待機モーションに
	if (!MoveEntry()) {
		pPlayer->ChangeState(new CStatePlayerIdle(pPlayer));
		return;
	}

	// アクション
	if (Action(pPlayer)) {
		return;
	}

	// 更新
	m_pStateMove->UpdateMoveState(this, pPlayer);

	// カウンター更新
	m_FrameCounter++;
}

bool CStatePlayerMove::Action(CPlayer* pPlayer)
{
	if (CInput::GetKeyTrigger(VK_SPACE) || CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A)) {

		// 10F経ってから回避可能に
		//if (m_FrameCounter >= 10) {
			pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
			return true;
		//}
	}

	// 攻撃ステートに遷移
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		//// 納刀時は抜刀ステートに遷移
		//if ((pPlayer->WeaponState() == SWORD_STATE_SHEATHE)) {

		//	pPlayer->ChangeState(new CStatePlayerDrawSword(pPlayer, true));
		//	return true;
		//}
		pPlayer->WeaponState() = SWORD_STATE_DRAW;
		pPlayer->ChangeState(new CStatePlayerAttack(pPlayer, PLAYER_STATE_ATTACK_JUMP));
		return true;
	}

	// 防御ステートに遷移
	if (CInput::GetGamepadPress(LEFT_TRRIGER) || CInput::GetKeyPress(VK_LSHIFT)) {

		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {
			pPlayer->ChangeState(new CStatePlayerBlock(pPlayer));
			return true;
		}
	}

	// 納刀ステートに遷移
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
