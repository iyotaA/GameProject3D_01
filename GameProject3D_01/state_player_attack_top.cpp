#include "game_objects_all.h"
#include "state_player_dodge.h"
#include "state_player_move.h"
#include "state_player_idle.h"
#include "state_player_attack.h"
#include "state_player_attack_top.h"
#include "state_player_attack_low.h"
#include "modelAnimation.h"
#include "player.h"

CStatePlayerAttackTop::CStatePlayerAttackTop(CPlayer* pPlayer)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(pPlayer->GetFront())
{
	pPlayer->SetAnimation(PLAYER_STATE_ATTACK_TOP, 1.0f);
	pPlayer->SetAnimationSpeed(1.0f);
}

CStatePlayerAttackTop::~CStatePlayerAttackTop()
{

}

void CStatePlayerAttackTop::Move(CPlayer* pPlayer)
{

}

void CStatePlayerAttackTop::UpdateAttackState(CStatePlayerAttack* pAttackState, CPlayer* pPlayer)
{
	// �ړ�����
	Move(pPlayer);

	if (pPlayer->AnimationBlending()) return;
	if (m_FrameCounter++ <= pPlayer->GetCurrentAnimFrameNum() - 80) return;

	pAttackState->ChangeState(new CStatePlayerAttackLow(pPlayer));
}