#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_top.h"
#include "state_player_attack_horizontal.h"
#include "state_player_attack_combo.h"
#include "player.h"

CStatePlayerAttackTop::CStatePlayerAttackTop(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_TOP, 1.0f);
	pPlayer->SetAnimationSpeed(1.75f);

	// 攻撃力設定
	pPlayer->AttackValue() = 42;
}

CStatePlayerAttackTop::~CStatePlayerAttackTop()
{

}

void CStatePlayerAttackTop::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);
	m_FrameCounter++;

	// アニメーションのブレンドが終わるまで以下の処理を実行させない
	if (pPlayer->AnimationBlending()) return;

	// 攻撃可能状態にする
	if (m_FrameCounter == 20) {

		CSound::Play(SOUND_LABEL_SE_SWING); // 効果音再生
		pPlayer->Attacked() = true;
	}

	// 入力可能までのフレーム
	if (m_FrameCounter <= 65)return;

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

		// *** コンボ攻撃 ***
		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackCombo(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** 回避 ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.75f) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackTop::Move(CPlayer* pPlayer)
{

}
