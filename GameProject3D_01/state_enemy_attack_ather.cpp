#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_attack.h"
#include "state_enemy_attack_hook.h"
#include "state_enemy_attack_jump.h"
#include "state_enemy_attack_ather.h"
#include "modelAnimation.h"
#include "enemy.h"

CStateEnemyAttackAther::CStateEnemyAttackAther(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_Velocity(Vector3())
	, m_DirFront(Vector3(0.0f, 0.0f, 1.0f))
{
	pEnemy->SetAnimation(ENEMY_STATE_ATTACK_ATHER, 1.0f);
	pEnemy->SetAnimationSpeed(1.25f);
}

CStateEnemyAttackAther::~CStateEnemyAttackAther()
{

}

void CStateEnemyAttackAther::UpdateAttackState(CStateEnemyAttack* pAttackState, CEnemy* pEnemy)
{

}

void CStateEnemyAttackAther::Move(CEnemy* pEnemy)
{

}
