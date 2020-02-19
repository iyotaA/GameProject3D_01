#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_combo.h"
#include "state_player_attack_horizontal.h"
#include "state_player_attack_jump.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackCombo::CStatePlayerAttackCombo(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_COMBO, 1.0f);
	pPlayer->SetAnimationSpeed(2.0f);

	// 攻撃力設定
	pPlayer->AttackValue() = 40;
}

CStatePlayerAttackCombo::~CStatePlayerAttackCombo()
{

}

void CStatePlayerAttackCombo::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);
	m_FrameCounter++;

	// アニメーションのブレンドが終わるまで以下の処理を実行させない
	if (pPlayer->AnimationBlending()) return;

	// 攻撃可能状態にする(60 ～ 70F)
	if (m_FrameCounter == 60) {

		CSound::Play(SOUND_LABEL_SE_SWING); // 効果音再生
		pPlayer->Attacked() = true;
	}
	if (m_FrameCounter == 70) {
		pPlayer->Attacked() = false;
	}

	// 攻撃可能状態にする(80 ～ 90F)
	if (m_FrameCounter == 80) {

		CSound::Play(SOUND_LABEL_SE_SWING); // 効果音再生
		pPlayer->Attacked() = true;
	}

	// 入力可能までのフレーム
	if (m_FrameCounter < 90)return;

	///////////////////////////////////////
	// キー入力で次の攻撃ステートに遷移
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		// *** 横スイング攻撃 ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackHorizontal(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_B) || CInput::GetKeyTrigger('O')) {

		// *** ジャンプ攻撃 ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackJump(pPlayer, true));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** 回避 ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	// 何も入力が無ければ待機状態に遷移
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 2.0) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackCombo::Move(CPlayer* pPlayer)
{

}
