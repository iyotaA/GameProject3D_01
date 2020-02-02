#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_combo.h"
#include "state_player_attack_horizontal.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackCombo::CStatePlayerAttackCombo(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_COMBO, 1.0f);
	pPlayer->SetAnimationSpeed(1.25f);
}

CStatePlayerAttackCombo::~CStatePlayerAttackCombo()
{

}

void CStatePlayerAttackCombo::Move(CPlayer* pPlayer)
{

}

void CStatePlayerAttackCombo::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// ˆÚ“®ˆ—
	Move(pPlayer);

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter++ <= pPlayer->GetCurrentAnimFrameNum() - 80) return;

	pAttackState->ChangeState(new CStatePlayerAttackHorizontal(pPlayer));
}
