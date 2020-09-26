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
#include "state_player_died.h"
#include "player.h"
#include "MathFunc.h"
#include "scene.h"
#include "enemy.h"

CStatePlayerMove::CStatePlayerMove(CPlayer* pPlayer)
	: m_pStateMove(new CStatePlayerMoveRun(pPlayer, 1.0f))
{
	pPlayer->Moved() = true;
}

CStatePlayerMove::~CStatePlayerMove()
{
	delete m_pStateMove;
}

void CStatePlayerMove::Update(CPlayer* pPlayer)
{
	// 死亡ステートに遷移
	if (pPlayer->Life() <= 0.0f) {
		pPlayer->ChangeState(new CStatePlayerDied(pPlayer));
		pPlayer->Moved() = false;
		return;
	}

	// ダメージステートに遷移
	if (pPlayer->Damaged()) {
		pPlayer->Dashed() = false;
		pPlayer->Moved() = false;
		CEnemy* enemy = CManager::GetScene()->GetGameObject<CEnemy>(CManager::LAYER_OBJECT);
		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer, enemy->Attack()));
		return;
	}

	// 入力がされてなければ待機モーションに
	if (!MoveEntry()) {
		pPlayer->Dashed() = false;
		pPlayer->Moved() = false;
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

		// スタミナが20以上あると回避ステートに遷移
		if ((pPlayer->Stamina() > 20)) {
			pPlayer->Dashed() = false;
			pPlayer->Moved() = false;
			pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
			return true;
		}
	}

	// 攻撃ステートに遷移
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		pPlayer->Dashed() = false;
		pPlayer->Moved() = false;

		// 抜刀時は縦攻撃
		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {
			pPlayer->ChangeState(new CStatePlayerAttack(pPlayer, PLAYER_STATE_ATTACK_VIRTICAL));
		}
		else if (pPlayer->WeaponState() == SWORD_STATE_SHEATHE) {	// 納刀時はジャンプ攻撃
			pPlayer->WeaponState() = SWORD_STATE_DRAW;
			pPlayer->ChangeState(new CStatePlayerAttack(pPlayer, PLAYER_STATE_ATTACK_JUMP));
		}
		return true;
	}

	// 防御ステートに遷移
	if (CInput::GetGamepadPress(LEFT_TRRIGER) || CInput::GetKeyPress(VK_LSHIFT)) {

		if ((pPlayer->WeaponState() == SWORD_STATE_DRAW)) {
			pPlayer->Dashed() = false;
			pPlayer->Moved() = false;
			pPlayer->ChangeState(new CStatePlayerBlock(pPlayer));
			return true;
		}
	}

	// 納刀ステートに遷移
	if ((pPlayer->WeaponState() == SWORD_STATE_DRAW) &&
		((CInput::GetGamepadTrigger(XINPUT_GAMEPAD_X) || CInput::GetKeyTrigger('K')))) {

		pPlayer->Dashed() = false;
		pPlayer->Moved() = false;
		pPlayer->ChangeState(new CStatePlayerSheatheSword(pPlayer, true));
		return true;
	}

	// アイテム使用
	if ((pPlayer->WeaponState() == SWORD_STATE_SHEATHE) &&
		((CInput::GetGamepadTrigger(XINPUT_GAMEPAD_X) || CInput::GetKeyTrigger('K')))) {

		pPlayer->UseItem();
	}

	return false;
}

void CStatePlayerMove::ChangeState(CStatePlayerMove* pMoveState)
{
	delete m_pStateMove;
	m_pStateMove = pMoveState;
}
