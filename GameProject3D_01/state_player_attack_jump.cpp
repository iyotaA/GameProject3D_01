#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_jump.h"
#include "state_player_attack_combo.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackJump::CStatePlayerAttackJump(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_JUMP, 1.0f);
	pPlayer->SetAnimationSpeed(1.25f);
}

CStatePlayerAttackJump::~CStatePlayerAttackJump()
{

}

void CStatePlayerAttackJump::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// ˆÚ“®ˆ—
	Move(pPlayer);

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter++ <= pPlayer->GetCurrentAnimFrameNum() - 115) return;

	pAttackState->ChangeState(new CStatePlayerAttackNone(pPlayer));
}

void CStatePlayerAttackJump::Move(CPlayer* pPlayer)
{
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
