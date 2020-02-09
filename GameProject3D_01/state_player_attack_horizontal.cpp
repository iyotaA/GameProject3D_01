#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_horizontal.h"
#include "state_player_attack_virtical.h"
#include "state_player_attack_low.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackHorizontal::CStatePlayerAttackHorizontal(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_HORIZONTAL, 1.0f);
	pPlayer->SetAnimationSpeed(1.25f);
}

CStatePlayerAttackHorizontal::~CStatePlayerAttackHorizontal()
{

}

void CStatePlayerAttackHorizontal::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);
	m_FrameCounter++;

	// 攻撃可能状態にする
	if (m_FrameCounter == 40) {

		CSound::Play(SOUND_LABEL_SE_SWING); // 効果音再生
		pPlayer->Attacked() = true;
	}

	// 入力可能までのフレーム
	if (m_FrameCounter <= 70)return;

	///////////////////////////////////////
	// キー入力で次の攻撃ステートに遷移
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackLow(pPlayer));
		return;
	}
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_B) || CInput::GetKeyTrigger('O')) {

		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackVirtical(pPlayer));
		return;
	}

	// アニメーションが終わり次第ステート遷移
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.25f) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackHorizontal::Move(CPlayer* pPlayer)
{

}
