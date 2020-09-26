#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_attack.h"
#include "state_player_attack_low.h"
#include "player.h"

CStatePlayerAttackLow::CStatePlayerAttackLow(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_LOW, 1.0f);
	pPlayer->SetAnimationSpeed(1.5f);

	// 攻撃力設定
	pPlayer->AttackValue() = 32;
}

CStatePlayerAttackLow::~CStatePlayerAttackLow()
{

}

void CStatePlayerAttackLow::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// 移動処理
	Move(pPlayer);
	m_FrameCounter++;

	// 攻撃可能状態にする
	if (m_FrameCounter == 40) {

		CSound::Play(SOUND_LABEL_SE_SWING); // 効果音再生
		pPlayer->Attacked() = true;
	}

	// 入力可能までのフレーム(F)
	if (m_FrameCounter < 60)return;

	if (CInput::GetGamepadTrigger(XINPUT_GAMEPAD_A) || CInput::GetKeyTrigger('K')) {

		// *** 回避 ***
		pPlayer->Attacked() = false;
		pPlayer->ChangeState(new CStatePlayerDodge(pPlayer));
		return;
	}


	// アニメーションのブレンドが終わるまで以下の処理を実行させない
	if (m_FrameCounter <= pPlayer->GetCurrentAnimFrameNum() / 1.5f - 10) return;

	pPlayer->Attacked() = false;
	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackLow::Move(CPlayer* pPlayer)
{

}
