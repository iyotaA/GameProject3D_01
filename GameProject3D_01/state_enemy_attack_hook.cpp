#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_attack_hook.h"
#include "modelAnimation.h"
#include "enemy.h"

CStateEnemyAttackHook::CStateEnemyAttackHook(CEnemy* pEnemy)
	: m_FrameCounter(0)
{
	pEnemy->SetAnimation(ENEMY_STATE_ATTACK_HOOK, 1.0f);
	pEnemy->SetAnimationSpeed(1.2f);

	// UŒ‚—ÍÝ’è
	pEnemy->Attack() = 25;
}

CStateEnemyAttackHook::~CStateEnemyAttackHook()
{

}

void CStateEnemyAttackHook::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{
	m_FrameCounter++;

	if (m_FrameCounter == 70) {		// UŒ‚—LŒø
		pEnemy->SetAnimationSpeed(3.0f);
		pEnemy->Attacked() = true;
	}
	else if (m_FrameCounter == 90) {		// UŒ‚–³Œø
		pEnemy->SetAnimationSpeed(1.5f);
		pEnemy->Attacked() = false;
	}

	if (m_FrameCounter <= 100) return;

	pEnemy->Attacked() = false;
	pAttackState->ChangeState(new CStateEnemyAttackNone(pEnemy));
}

void CStateEnemyAttackHook::Move(CEnemy* pEnemy)
{

}
