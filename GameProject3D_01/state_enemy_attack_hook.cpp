#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_attack_hook.h"
#include "modelAnimation.h"
#include "enemy.h"

CStateEnemyAttackHook::CStateEnemyAttackHook(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(Vector3(0.0f, 0.0f, 1.0f))
{
	pEnemy->SetAnimation(ENEMY_STATE_ATTACK_HOOK, 1.0f);
	pEnemy->SetAnimationSpeed(1.2f);
}

CStateEnemyAttackHook::~CStateEnemyAttackHook()
{

}

void CStateEnemyAttackHook::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{
}

void CStateEnemyAttackHook::Move(CEnemy* pEnemy)
{

}
