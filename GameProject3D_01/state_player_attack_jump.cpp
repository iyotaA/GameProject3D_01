#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "state_player_attack_combo.h"
#include "state_player_attack_top.h"
#include "state_player_damage_large.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackJump::CStatePlayerAttackJump(CPlayer* pPlayer, bool isMove)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
	, m_Move(isMove)
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_JUMP, 1.0f);
	pPlayer->SetAnimationSpeed(1.5f);
}

CStatePlayerAttackJump::~CStatePlayerAttackJump()
{

}

void CStatePlayerAttackJump::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// ダメージステートに遷移
	if (pPlayer->Damaged()) {

		pPlayer->ChangeState(new CStatePlayerDamage(pPlayer));
		return;
	}

	// 移動処理
	Move(pPlayer);
	m_FrameCounter++;

	// アニメーションのブレンドが終わるまで以下の処理を実行させない
	if (pPlayer->AnimationBlending()) return;

	// 攻撃可能状態にする
	if (m_FrameCounter == 50) {	pPlayer->Attacked() = true;	}

	// 効果音再生
	if (m_FrameCounter == 60) {	CSound::Play(SOUND_LABEL_SE_SWING); }

	// 入力可能までのフレーム
	if (m_FrameCounter <= 90)return;

	///////////////////////////////////////
	// キー入力で次の攻撃ステートに遷移
	///////////////////////////////////////
	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_Y) || CInput::GetKeyTrigger('I')) {

		pPlayer->Attacked() = false;
		pAttackState->ChangeState(new CStatePlayerAttackCombo(pPlayer));
		return;
	}

	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() - 80) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackJump::Move(CPlayer* pPlayer)
{
	if (!m_Move)return;

	if (m_FrameCounter >= 68)return;

	if (m_FrameCounter <= 25) {
		m_Velocity += m_DirFront * pPlayer->DefaultSpeed();
		m_Velocity *= 0.88f;
		pPlayer->AddVelocity(m_Velocity);
		return;
	}

	m_Velocity *= 0.99f;
	pPlayer->AddVelocity(m_Velocity);
}
